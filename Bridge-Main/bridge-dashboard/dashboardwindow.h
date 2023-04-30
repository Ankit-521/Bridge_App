#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <serverlistener.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class DashboardWindow;
}
QT_END_NAMESPACE

class DashboardWindow : public QMainWindow {
  Q_OBJECT

public:
  DashboardWindow(QWidget *parent = nullptr);
  ~DashboardWindow();

  Q_INVOKABLE inline void restoreWindow() { setWindowState(Qt::WindowNoState); }

  /// @brief Toggles full screen
  Q_INVOKABLE void toggleFullScreen();

private:
  Ui::DashboardWindow *ui;
  ServerListener *serverListener;
};
#endif // DASHBOARDWINDOW_H
