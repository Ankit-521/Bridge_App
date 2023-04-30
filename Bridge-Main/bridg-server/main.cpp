#include "mainwindow.h"
#include <QApplication>

#include "mainvideochatwindow.h"

// Code changed by Wasswa Geoffrey
//#include "logindialog.h" //Added by PVD
#include <authmanager.h>

int main(int argc, char *argv[]) {

  MainVideoChatWindow::initialiseWindow();
  QApplication a(argc, argv);
  //  LoginDialog ld;
  /// ld.show();

  MainWindow *w = new MainWindow; // Commented by PVD
                                  // Commented by PVD
  w->show();
  //  AuthManager authManager;
  //  QObject::connect(&authManager, &AuthManager::signInChanged, [&]() {
  //    if (authManager.signedIn()) {
  //      authManager.close();

  //      w->show();
  //    }
  //  });

  //  authManager.setIsDirector(true);
  //  authManager.show();

  return a.exec();
}
