
// Code changed by Wasswa Geoffrey
//#include <QDataStream>
//#include <QIODevice>

#include <QJsonDocument>
#include <QNetworkInterface>
#include "networkutils.h"

NetworkUtils::NetworkUtils(QObject *parent) :
    QObject(parent)
{
}

void reportError(QString err, QString data = QString())
{
    if (data.isEmpty())
        qDebug() << "Error receiving: " << err;
    else
        qDebug() << "Error sending: " << data << ": " << err;
}

QByteArray NetworkUtils::wrapString(QString message)
{

    // code changed by Wasswa Geoffrey
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out << message;
//    return block;

     QByteArray block;
      QDataStream out(&block, QIODevice::WriteOnly);
      out << QVariant(message);

      return block;

}

void NetworkUtils::broadcast(QString msg, int port)
{
    QUdpSocket socket;
    if (socket.writeDatagram(wrapString(msg), QHostAddress::Broadcast, port) == -1) {
        reportError(socket.errorString());
        return;
    }
    socket.close();
    qDebug() << "UDP-SEND:" << msg;
}

QString NetworkUtils::readBroadcast(QUdpSocket *sock, QHostAddress *senderPtr, bool readFromLocal)
{
    if (!sock) return QString();
    QHostAddress sender;
    QString data;

    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        if (sock->readDatagram(datagram.data(), datagram.size(), &sender) == -1) {
            reportError(sock->errorString());
            continue;
        }
        if (!QNetworkInterface::allAddresses().contains(sender) || readFromLocal) {
            QDataStream in(&datagram, QIODevice::ReadOnly);
            // Code changed by Wasswa Geoffrey
            //QString msg;

            QVariant msg;
            in >> msg;
            data += msg.toString();

            qDebug()<<"in read broadcast ";
            qDebug() << "UDP-RECV:" << msg;
        }
    }
    if (senderPtr) *senderPtr = sender;
    return data;
}

void NetworkUtils::tcpSend(QString msg, QTcpSocket *sock)
{
    if (!sock)
        return;
    if (sock->write(wrapString(msg)) == -1) {
        reportError(sock->errorString(), msg);
        return;
    }
    qDebug() << "TCP-SEND:" << msg;
}

QString NetworkUtils::tcpRecv(QTcpSocket *sock)
{

    QDataStream in(sock);

    // Code changed by Wasswa Geoffrey
    //QString msg;

    QVariant msg;
    in >> msg;
    qDebug() << "TCP-RECV:" << msg;
    return msg.toString();
}

void NetworkUtils::multicast(QString msg, QList<QTcpSocket*> sockets, int except)
{
    for (int i=0; i<sockets.size(); i++) {
        if (i==except || !sockets[i]) continue;
        tcpSend(msg, sockets[i]);
    }
}



