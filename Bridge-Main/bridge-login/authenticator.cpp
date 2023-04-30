#include "authenticator.h"
#include <QDebug>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QOAuthHttpServerReplyHandler>
#include <QUrl>
#include <QVariantMap>
#include <bridgeutils.h>

#define SERVER_LOCATION "http://localhost/bridge/auth.php"
#define PLAYER_CREDENTIAL_FILENAME "PLAYER.dat"

Authenticator::Authenticator(QObject *parent)
    : QObject(parent), playerProfile(new Profile),
      googleAuthCodeFlow(new QOAuth2AuthorizationCodeFlow()),
      m_NetworkAccessManager(new QNetworkAccessManager),
      m_keepMeSignedIn(false), m_requestType(NO_REQUEST) {

  connect(m_NetworkAccessManager, &QNetworkAccessManager::finished, this,
          &Authenticator::finished);
  connect(googleAuthCodeFlow, &QOAuth2AuthorizationCodeFlow::granted, this,
          &Authenticator::fetchPlayerAccountDetails);
  connect(googleAuthCodeFlow, &QOAuth2AuthorizationCodeFlow::error, this,
          [this](const QString &error, const QString &description,
                 const QUrl &uri) {
            qDebug() << "error is " << error << "  " << description << "s   "
                     << uri;
            emit errorChanged(AUTHENTICATION_ERROR, description);
          });

  // Open in  browser
  connect(googleAuthCodeFlow,
          &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          &QDesktopServices::openUrl);
}

void Authenticator::signIn(const Authenticator::Credentials &credential,
                           const QString &usernames, const QString &password) {

  switch (credential) {
  case Google:
    requestGoogleAppDetails();
    break;
  case REMOTE_SQL: {
    signInPlayer(usernames, password);
    break;
  }
  }
}

QNetworkRequest Authenticator::constructNetworkRequest() {
  QNetworkRequest request;
  request.setUrl(QUrl(SERVER_LOCATION));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  return request;
}

void Authenticator::signInPlayer(const QString &playerNames,
                                 const QString &password) {

  if (!playerNames.isEmpty() && !password.isEmpty()) {

    QVariantMap playerMap = QVariantMap();
    playerMap["name"] = playerNames;
    playerMap["password"] = password;

    QJsonDocument doc = QJsonDocument::fromVariant(playerMap);

    QByteArray dbData = "player=";
    dbData.append(doc.toJson().toStdString().c_str());

    auto request = constructNetworkRequest();

    setResponseType(AUTHENTICATE_PLAYER);
    m_NetworkAccessManager->post(request, dbData);

  } else {
    errorChanged(INVALID_DETAILS_ERROR, "Usernames or password is empty");
  }
}

void Authenticator::requestGoogleAppDetails() {

  auto request = constructNetworkRequest();

  auto url = request.url().toString();

  qDebug() << "My url befor is  " << url;
  // query by app ame
  url += "?app=contact_bridge";
  qDebug() << "My url is  " << url;

  request.setUrl(QUrl(url));
  setResponseType(FETCH_APP_DETAILS);
  m_NetworkAccessManager->get(request);
}

void Authenticator::checkIfSignedIn() {
  auto playerCredentials = BridgeUtils::readData(PLAYER_CREDENTIAL_FILENAME);
  if (!playerCredentials.isEmpty()) {

    m_keepMeSignedIn = false; // prevent rewriting of player data
    handleCredentials(playerCredentials);
    authenticationComplete(true);
  }
}

void Authenticator::handleCredentials(const QVariantMap &profileData) {

  if (!profileData.isEmpty()) {
    if (profileData.contains("name") && profileData.contains("email") &&
        profileData.contains("picture")) {
      playerProfile->setPlayerName(profileData.find("name").value().toString());
      playerProfile->setPlayerEmail(
          profileData.find("email").value().toString());
      playerProfile->setPlayerPicture(
          profileData.find("picture").value().toString());

      emit playerProfileChanged(playerProfile);

      if (m_keepMeSignedIn) {

        // Persist player credentials
        if (!BridgeUtils::storeData(profileData, PLAYER_CREDENTIAL_FILENAME)) {
          qWarning() << "Failed to save data";
        }
      }
    }
  }
}

void Authenticator::intialiseGoogleAuth(const QJsonDocument &appDetails) {

  qDebug() << "My app destils is  " << appDetails;
  if (!appDetails.isEmpty()) {
    googleAuthCodeFlow->setScope("email");

    const auto object = appDetails.object();
    if (!object.isEmpty()) {
      const auto settingsObject = object["web"].toObject();

      const QUrl authUri(settingsObject["auth_uri"].toString());
      const auto clientId = settingsObject["client_id"].toString();
      const auto tokenUri(settingsObject["token_uri"].toString());
      auto clientSecret(settingsObject["client_secret"].toString());

      auto redirectUris = settingsObject["redirect_uris"].toArray();
      const QUrl redirectUri(redirectUris[0].toString());
      const auto port = static_cast<quint16>(redirectUri.port());

      googleAuthCodeFlow->setAuthorizationUrl(authUri);
      googleAuthCodeFlow->setClientIdentifier(clientId);
      googleAuthCodeFlow->setAccessTokenUrl(tokenUri);
      googleAuthCodeFlow->setClientIdentifierSharedKey(clientSecret);

      auto replyHandler = new QOAuthHttpServerReplyHandler(port, this);
      googleAuthCodeFlow->setReplyHandler(replyHandler);

      connect(replyHandler, &QOAuthHttpServerReplyHandler::tokensReceived, this,
              [this](const QVariantMap &tokens) {
                m_authTokens["google"] = tokens;
              });

      googleAuthCodeFlow->setModifyParametersFunction(
          [](QAbstractOAuth::Stage stage, QVariantMap *parameters) {
            // Percent-decode the "code" parameter so Google can match it
            if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
              QByteArray code = parameters->value("code").toByteArray();
              (*parameters)["code"] = QUrl::fromPercentEncoding(code);
            }
          });

      googleAuthCodeFlow->grant();
    } else {
      // TODO
      // Don`t expose internal errors to player
      // Write them to the app log
      qWarning() << __FUNCTION__ << __LINE__
                 << "Could not parse an empty object " << object;

      emit errorChanged(UNKNOWN_ERROR, "an unknown error has occured.");
    }
  } else {
    // TODO
    // Don`t expose internal errors to player
    // Write them to the app log
    qWarning() << __FUNCTION__ << __LINE__
               << "Could not parse an empty QDocument " << appDetails;

    emit errorChanged(UNKNOWN_ERROR, "an unknown error has occured.");
  }
}

void Authenticator::handlePlayerAuthentication(const QByteArray &response) {
  QJsonDocument doc = QJsonDocument::fromJson(response);
  if (!doc.isEmpty()) {

    auto response_as_map = doc.toVariant().toMap();
    if (!response_as_map.isEmpty()) {

      auto error = response_as_map.find("error").value().toString();
      auto response_profile = response_as_map.find("profile").value().toMap();

      if (!error.isEmpty()) {

        emit errorChanged(INVALID_DETAILS_ERROR, error);
        authenticationComplete(false);

      } else {

        emit authenticationComplete(true);
        handleCredentials(response_profile);
      }
    }
  } else {
    emit errorChanged(UNKNOWN_ERROR, "an unknown error has occured.");
    authenticationComplete(false);
  }
}

void Authenticator::finished(QNetworkReply *reply) {

  if (reply && reply->error() == QNetworkReply::NoError) {
    QByteArray response(reply->readAll());

    switch (m_requestType) {
    case AUTHENTICATE_PLAYER:
      handlePlayerAuthentication(response);
      break;
    case FETCH_APP_DETAILS:
      this->intialiseGoogleAuth(QJsonDocument::fromJson(response));
      break;
    case FETCH_PLAYER_PROFILE:
      break;
    default: {}
    }

  } else {

    // TODO
    // Don`t expose internal errors to player
    // Write them to the app log
    qWarning() << __FUNCTION__ << __LINE__ << reply->errorString();

    emit errorChanged(UNKNOWN_ERROR, "an unknown error has occured.");
  }
}

void Authenticator::fetchPlayerAccountDetails() {
  // TODO
  QNetworkRequest request;
  QString url =
      "https://www.googleapis.com/oauth2/v2/userinfo?alt=json&access_token=" +
      getAuthTokens(Google).value("").toString();
  qDebug() << "My url is  " << url;
  request.setUrl(QUrl(url));
}

void Authenticator::setResponseType(const ResponseType &requestType) {
  m_requestType = requestType;
}

QVariantMap Authenticator::getAuthTokens(Credentials cred) const {
  QString credential = "";
  switch (cred) {
  case Google:
    credential = "google";
    break;
  default:
    credential = "none";
  }

  return (credential == "none") ? QVariantMap()
                                : m_authTokens.value(credential).toMap();
}

bool Authenticator::getKeepMeSignedIn() const { return m_keepMeSignedIn; }

void Authenticator::setKeepMeSignedIn(bool keepMeSignedIn) {
  m_keepMeSignedIn = keepMeSignedIn;
}

Profile *Authenticator::getPlayerProfile() const { return playerProfile; }

////////////////////////////////////////////////////////////////////////
//////////////////////// Profile ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////

Profile::Profile() { initialise(); }

Profile::~Profile() {}

Profile::Profile(const Profile &other) {
  initialise();
  setPlayerEmail(other.m_PlayerEmail);
  setPlayerPicture(other.m_PlayerPicture);
  setPlayerName(other.m_PlayerName);
}

Profile &Profile::operator=(const Profile &other) {

  auto m_other = const_cast<Profile *>(&other);
  m_other->setPlayerEmail(m_PlayerEmail);
  m_other->setPlayerPicture(m_PlayerPicture);
  m_other->setPlayerName(m_PlayerName);
  return *m_other;
}

QString Profile::playerEmail() const { return m_PlayerEmail; }

void Profile::setPlayerEmail(const QString &playerEmail) {
  m_PlayerEmail = playerEmail;
}

QString Profile::playerName() const { return m_PlayerName; }

void Profile::setPlayerName(const QString &playerName) {
  m_PlayerName = playerName;
}

QUrl Profile::playerPicture() const { return m_PlayerPicture; }

void Profile::setPlayerPicture(const QUrl &playerPicture) {
  m_PlayerPicture = playerPicture;
}

void Profile::initialise() {
  typeId = qRegisterMetaType<Profile>();
  m_PlayerName = m_PlayerEmail = "";
  m_PlayerPicture = QUrl("");
}

int Profile::getTypeId() const { return typeId; }
