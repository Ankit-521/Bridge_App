/********************************************************************************
** Form generated from reading UI file 'infodialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFODIALOG_H
#define UI_INFODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_InfoDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *tableSpin;
    QRadioButton *northRadio;
    QRadioButton *eastRadio;
    QRadioButton *southRadio;
    QRadioButton *westRadio;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;
    QLabel *statusLabel;
    QRadioButton *specRadio;

    void setupUi(QDialog *InfoDialog)
    {
        if (InfoDialog->objectName().isEmpty())
            InfoDialog->setObjectName(QStringLiteral("InfoDialog"));
        InfoDialog->resize(286, 154);
        gridLayout = new QGridLayout(InfoDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 1);
        label = new QLabel(InfoDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(InfoDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 3, 1);

        tableSpin = new QSpinBox(InfoDialog);
        tableSpin->setObjectName(QStringLiteral("tableSpin"));
        tableSpin->setMinimum(1);

        gridLayout->addWidget(tableSpin, 0, 1, 1, 2);

        northRadio = new QRadioButton(InfoDialog);
        northRadio->setObjectName(QStringLiteral("northRadio"));

        gridLayout->addWidget(northRadio, 1, 1, 1, 1);

        eastRadio = new QRadioButton(InfoDialog);
        eastRadio->setObjectName(QStringLiteral("eastRadio"));

        gridLayout->addWidget(eastRadio, 1, 2, 1, 1);

        southRadio = new QRadioButton(InfoDialog);
        southRadio->setObjectName(QStringLiteral("southRadio"));

        gridLayout->addWidget(southRadio, 2, 1, 1, 1);

        westRadio = new QRadioButton(InfoDialog);
        westRadio->setObjectName(QStringLiteral("westRadio"));

        gridLayout->addWidget(westRadio, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 1, 1, 1);

        buttonBox = new QDialogButtonBox(InfoDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 5, 0, 1, 3);

        statusLabel = new QLabel(InfoDialog);
        statusLabel->setObjectName(QStringLiteral("statusLabel"));

        gridLayout->addWidget(statusLabel, 6, 0, 1, 3);

        specRadio = new QRadioButton(InfoDialog);
        specRadio->setObjectName(QStringLiteral("specRadio"));

        gridLayout->addWidget(specRadio, 3, 1, 1, 2);


        retranslateUi(InfoDialog);
        QObject::connect(buttonBox, SIGNAL(rejected()), InfoDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(InfoDialog);
    } // setupUi

    void retranslateUi(QDialog *InfoDialog)
    {
        InfoDialog->setWindowTitle(QApplication::translate("InfoDialog", "Enter your details", nullptr));
        label->setText(QApplication::translate("InfoDialog", "Table:", nullptr));
        label_2->setText(QApplication::translate("InfoDialog", "Player:", nullptr));
        northRadio->setText(QApplication::translate("InfoDialog", "North", nullptr));
        eastRadio->setText(QApplication::translate("InfoDialog", "East", nullptr));
        southRadio->setText(QApplication::translate("InfoDialog", "South", nullptr));
        westRadio->setText(QApplication::translate("InfoDialog", "West", nullptr));
        statusLabel->setText(QString());
        specRadio->setText(QApplication::translate("InfoDialog", "Spectator", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InfoDialog: public Ui_InfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFODIALOG_H
