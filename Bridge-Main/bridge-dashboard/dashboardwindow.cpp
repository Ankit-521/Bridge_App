#include "dashboardwindow.h"
#include "ui_dashboardwindow.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <bridgeutils.h>
DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DashboardWindow) {
  ui->setupUi(this);

  serverListener = new ServerListener;

  auto qmlContext = ui->quickWidget->engine()->rootContext();
  qmlContext->setContextProperty("mainWindow", this);

  qmlContext->setContextProperty("serverListner", serverListener);
  BridgeUtils::set_up_quick_widget(
      ui->quickWidget, QUrl(QStringLiteral("qrc:/qml/Desktop.qml")));
  ui->quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

  // start in maximised screen
  setWindowState(Qt::WindowMaximized);
}

DashboardWindow::~DashboardWindow() { delete ui; }

void DashboardWindow::toggleFullScreen() {
  bool isFullScreen = (windowState() == Qt::WindowFullScreen);
  if (!isFullScreen)
    setWindowState(Qt::WindowFullScreen);
  else
    setWindowState(Qt::WindowNoState);
}
