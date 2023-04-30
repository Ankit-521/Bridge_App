#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QQuickWidget>
#include <QUrl>

/// @brief Helper function to set qquickWidget attributes and source url
void set_up_quick_widget(QQuickWidget *qquickWidget, const QUrl url);

#endif // UTILS_H
