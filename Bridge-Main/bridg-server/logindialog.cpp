#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <QDebug>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("remotemysql.com");
    db.setPort(3306);

    db.setUserName("m2H6QlRpXC");
    db.setPassword("6RufKFvmGy");
    db.setDatabaseName("m2H6QlRpXC");
    bool ok = db.open();
    if (ok) qDebug() << "Database opened" << endl;
    else qDebug()<< "Database not opened" << endl;




    /* Original code : changed by Chanchal Dass on 30th January 221
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");

    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("cdass01");
    bool ok = db.open();
    if (ok) qDebug() << "Database opened" << endl;
    else qDebug()<< "Database not opened" << endl;
*/
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_submitPB_clicked()
{

    MainWindow *w = new MainWindow();
    w->show();
    this->close();
    return;

    QSqlQuery query;

    query.exec("select password from user1 WHERE username like '" + ui->userLineEdit->text()   + "'");

    if (query.first())
    {
        if ((query.value(0).toString().compare(ui->passwordLineEdit->text()))==0)
        {
            qDebug()<<"Found user"<<endl;
            MainWindow *w = new MainWindow();
            w->show();
            this->close();
        }

        else
        {
            qDebug() << "Wrong password" << endl;

        }
    }
    else
    {
        qDebug() << "Wrong username" << endl;
    }
}
