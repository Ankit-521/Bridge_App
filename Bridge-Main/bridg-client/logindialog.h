#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include "gameengine.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_submitPB_clicked();

private:
    Ui::LoginDialog *ui;
    GameEngine *engine;
};

#endif // LOGINDIALOG_H
