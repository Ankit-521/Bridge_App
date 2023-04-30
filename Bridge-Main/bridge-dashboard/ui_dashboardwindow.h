/********************************************************************************
** Form generated from reading UI file 'dashboardwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DASHBOARDWINDOW_H
#define UI_DASHBOARDWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtQuickWidgets/QQuickWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DashboardWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QQuickWidget *quickWidget;

    void setupUi(QMainWindow *DashboardWindow)
    {
        if (DashboardWindow->objectName().isEmpty())
            DashboardWindow->setObjectName(QString::fromUtf8("DashboardWindow"));
        DashboardWindow->resize(1050, 650);
        QFont font;
        font.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        DashboardWindow->setFont(font);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/common_assets/images/bridge-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        DashboardWindow->setWindowIcon(icon);
        centralwidget = new QWidget(DashboardWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        quickWidget = new QQuickWidget(centralwidget);
        quickWidget->setObjectName(QString::fromUtf8("quickWidget"));
        quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        verticalLayout->addWidget(quickWidget);

        DashboardWindow->setCentralWidget(centralwidget);

        retranslateUi(DashboardWindow);

        QMetaObject::connectSlotsByName(DashboardWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DashboardWindow)
    {
        DashboardWindow->setWindowTitle(QCoreApplication::translate("DashboardWindow", "Bridge", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DashboardWindow: public Ui_DashboardWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DASHBOARDWINDOW_H
