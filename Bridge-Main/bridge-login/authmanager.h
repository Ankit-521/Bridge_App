#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <Bridge-login_global.h>
#include <QMainWindow>
#include <QQuickWidget>
#include <authenticator.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class AuthManager;
}
QT_END_NAMESPACE

/// @brief AuthManager provides a convinient way to handle authentication in the
/// app
class AUTHMANAGER_EXPORT AuthManager : public QMainWindow {

  Q_OBJECT

  Q_PROPERTY(
      bool isDirector MEMBER m_isDirector CONSTANT NOTIFY isDirectorChanged)
  Q_PROPERTY(bool signedIn MEMBER m_signedIn CONSTANT NOTIFY signInChanged)

  /// @brief Makes the window frameless
  void makeFrameless();

  /// @brief Adds a show to the window
  void AddShadow();

public:
  AuthManager(QWidget *parent = nullptr);
  ~AuthManager();

  /// @brief Returns true if this auth window is for the director, false
  /// otherwise
  bool isDirector() const;

  /// @brief Sets the director
  void setIsDirector(bool isDirector);

  /// @brief Returns true if user is signed in, false otherwise
  bool signedIn() const;

  /// @brief Emits AboutToClose signal and closes the window and emits. Please
  /// use this function to close instead of QMainwindow::close
  Q_INVOKABLE void closeWindow();

  Q_INVOKABLE void signIn(int credential, const QString &userNames = "",
                          const QString password = "");

  Q_INVOKABLE inline void keepMeSignedIn(bool yes) {
    auth->setKeepMeSignedIn(yes);
  }
  Q_INVOKABLE inline void checkIfSignedIn() { auth->checkIfSignedIn(); }

signals:
  void aboutToClose();
  void isDirectorChanged();
  void signInChanged();
  void handleError(int errorCode, const QString &errorMsg);

protected:
  /// @brief Initialises the content area of the window,
  /// It mainly comprises of two widget(layers) ie background and a
  /// qquickWidget.
  void setUpContentArea();

private:
  Ui::AuthManager *ui;
  QQuickWidget *qquickContentWidget;
  bool m_isDirector;
  bool m_signedIn;
  Authenticator *auth;
};
#endif // AUTHMANAGER_H
