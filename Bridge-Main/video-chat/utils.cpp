#include "utils.h"

void set_up_quick_widget(QQuickWidget *qquickWidget, const QUrl url) {

  if (qquickWidget != nullptr) {
    qquickWidget->setSource(url);
    qquickWidget->setAttribute(Qt::WA_TranslucentBackground);
    qquickWidget->setWindowFlags(Qt::FramelessWindowHint);
  }
}
