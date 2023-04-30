#include "authmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthManager w;
    w.show();
    return a.exec();
}
