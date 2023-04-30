#include "gameengine.h"
#include <QApplication>

// Code changed by Wasswa Geoffrey
//#include "logindialog.h" //Added by PVD
#include <QObject>
#include <QtWebEngine>
#include <authmanager.h>
// This is my first trial in ASUS Desktop Github
int main(int argc, char *argv[]) {

  QtWebEngine::initialize(); // Essential for web engine to work in QML

  QApplication app(argc, argv);

  // LoginDialog ld;
  // ld.show();
  GameEngine engine; // Commented by PVD
  engine.start();
  //  AuthManager authManager;
  //  authManager.show();
  //  QObject::connect(&authManager, &AuthManager::signInChanged, [&]() {
  //    qDebug() << "im sifnhe dis  " << authManager.signedIn();
  //    if (authManager.signedIn()) {
  //      authManager.close();
  //      engine.start();
  //    }
  //  });

  return app.exec();
}
