#include "authmanager.h"
#include "ui_authmanager.h"
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <bridgeutils.h>
void AuthManager::makeFrameless() {
#if defined(Q_OS_WIN)
  setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
  setAttribute(Qt::WA_NoSystemBackground, true);
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint);
}

void AuthManager::AddShadow() {
  QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect(this);
  windowShadow->setBlurRadius(9.0);
  windowShadow->setColor(QColor(0, 0, 0, 160));
  windowShadow->setOffset(0);
  ui->shadowWidget->setGraphicsEffect(windowShadow);
}

void AuthManager::setUpContentArea() {
  int margin = 8;

  // background widget that will contain the content area widget
  auto background = new QWidget(this);
  QRect geom(((width() / 2) - ((width() - margin * 2) / 2)),
             ((height() / 2) - ((height() - margin * 2) / 2)),
             width() - margin * 2, height() - margin * 2);

  background->setGeometry(geom);

  background->setStyleSheet("background:white");
  background->setAttribute(Qt::WA_AlwaysStackOnTop);

  QVBoxLayout *bg_layout = new QVBoxLayout(background);
  bg_layout->setContentsMargins(0, 0, 0, 0);
  bg_layout->setSpacing(0);

  // The content widget
  qquickContentWidget = new QQuickWidget(background);

  qquickContentWidget->engine()->rootContext()->setContextProperty(
      "authManager", this);

  BridgeUtils::set_up_quick_widget(
      qquickContentWidget, QUrl(QStringLiteral("qrc:/qml/Desktop.qml")));
  qquickContentWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

  bg_layout->addWidget(qquickContentWidget);
}

bool AuthManager::signedIn() const { return m_signedIn; }

void AuthManager::closeWindow() {
  emit aboutToClose();
  close();
}

void AuthManager::signIn(int credential, const QString &userNames,
                         const QString password) {
  auth->signIn(static_cast<Authenticator::Credentials>(credential), userNames,
               password);
}

bool AuthManager::isDirector() const { return m_isDirector; }

void AuthManager::setIsDirector(bool isDirector) {
  if (m_isDirector != isDirector) {
    m_isDirector = isDirector;
    emit isDirectorChanged();
  }
}
AuthManager::AuthManager(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AuthManager), auth(new Authenticator) {
  ui->setupUi(this);
  m_isDirector = false;
  m_signedIn = false;

  makeFrameless();
  AddShadow();

  setUpContentArea();

  // handle errors
  connect(auth, &Authenticator::errorChanged, this,
          [=](Authenticator::Errors error, const QString &errorMsg) {
            emit handleError(static_cast<int>(error), errorMsg);
          });

  // handle auths
  connect(auth, &Authenticator::authenticationComplete, this, [=](bool sucess) {
    m_signedIn = sucess;
    emit signInChanged();
  });
}

AuthManager::~AuthManager() {
  auth->deleteLater();
  delete ui;
}
