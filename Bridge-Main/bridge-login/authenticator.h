#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QNetworkAccessManager>
#include <QOAuth2AuthorizationCodeFlow>
#include <QObject>

/// @brief A struct that contains player profile information
struct Profile;

/// @brief A class that manages auths, Credentials and player profile
class Authenticator : public QObject {

  Q_OBJECT
public:
  /// @brief Enum that provides credential types used for logging in.
  enum Credentials {
    Google,
    REMOTE_SQL,
  };

  enum Errors {
    NONE,
    AUTHENTICATION_ERROR,
    NETWORK_ERROR,
    INVALID_DETAILS_ERROR,
    UNKNOWN_ERROR

  };
  explicit Authenticator(QObject *parent = nullptr);
  /// @brief Sign in player with a credential.
  void signIn(const Credentials &credential, const QString &usernames,
              const QString &password);

  /// @brief returns the player profile object.
  Profile *getPlayerProfile() const;

  /// @brief Check if player profile should be persistent.
  bool getKeepMeSignedIn() const;

  /// @brief Set whether player profile should be persistent.
  void setKeepMeSignedIn(bool keepMeSignedIn);

  /// @brief Intialises player profile and persists data if possible.
  void handleCredentials(const QVariantMap &profileData);

  /// @brief Checks if player account details are stored.
  void checkIfSignedIn();

  /// @brief Returns the stored token of a given Credential.
  QVariantMap getAuthTokens(Credentials cred) const;

signals:
  void errorChanged(const Errors &errorcode, const QString &description);
  void authenticationComplete(bool success);
  void playerProfileChanged(Profile *playerProfile);

protected:
  enum ResponseType {
    NO_REQUEST,
    AUTHENTICATE_PLAYER,
    FETCH_APP_DETAILS,
    FETCH_PLAYER_PROFILE
  };

  /// @brief initalises google signin code flow in the app.
  void intialiseGoogleAuth(const QJsonDocument &appDetails);

  /// @brief Signs in the player.
  void signInPlayer(const QString &userNames, const QString &pasword);

  /// @brief Requests google app data from the server.
  void requestGoogleAppDetails();

  /// @brief Constructs a network request to be sent to the php file.
  QNetworkRequest constructNetworkRequest();

  /// @brief Sets the request type of any given request.
  void setResponseType(const Authenticator::ResponseType &requestType);

  /// @brief Handles player authentication response from the server/network
  void handlePlayerAuthentication(const QByteArray &response);

protected slots:

  void finished(QNetworkReply *reply);

  /// @brief Fetches player google account details
  void fetchPlayerAccountDetails();

private:
  Profile *playerProfile;
  QNetworkAccessManager *m_NetworkAccessManager;
  QOAuth2AuthorizationCodeFlow *googleAuthCodeFlow;
  bool m_keepMeSignedIn;
  QVariantMap m_authTokens;
  ResponseType m_requestType;
};

struct Profile {
public:
  Profile();
  ~Profile();

  Profile(const Profile &other);
  Profile &operator=(const Profile &other);

  /// @brief Returns the player email
  QString playerEmail() const;

  /// @brief Sets the player email
  void setPlayerEmail(const QString &playerEmail);

  /// @brief Returns the player name
  QString playerName() const;

  /// @brief Sets the player name
  void setPlayerName(const QString &playerName);

  /// @brief Returns a url of the player picture
  QUrl playerPicture() const;

  /// @brief Sets the player picture
  void setPlayerPicture(const QUrl &playerPicture);

  /// @brief Returns type id of this object, eg Checking if this object can
  /// cast to a QVariant;
  int getTypeId() const;

protected:
  void initialise();

private:
  QString m_PlayerEmail;
  QString m_PlayerName;
  QUrl m_PlayerPicture;
  int typeId;
};
// make it parsable with QVariant
Q_DECLARE_METATYPE(Profile)

#endif // AUTHENTICATOR_H
