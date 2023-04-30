#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>

#include <QDataStream>
#include <QIODevice>

class NetworkUtils : public QObject
{
    Q_OBJECT
public:
    explicit NetworkUtils(QObject *parent = 0);
    static QByteArray wrapString(QString msg);
    static void broadcast(QString msg, int port);
    static QString readBroadcast(QUdpSocket *sock, QHostAddress *senderPtr = 0, bool readFromLocal = false);
    static void tcpSend(QString msg, QTcpSocket *sock);
    static QString tcpRecv(QTcpSocket *sock);
    static void multicast(QString msg, QList<QTcpSocket*> receivers, int except = -1);
    
    // code added by Wasswa Geoffrey

    /// @brief Helper function to serialise data of type T for sending from the server
    /// @param type T data to be serialised
    /// @note Its recomended to use data can be converted to a QVariant
     template<typename T>
    static QByteArray streamOut(const T &data)
    {
     QByteArray bytedata=QByteArray();
     QDataStream outStream(&bytedata,QIODevice::WriteOnly);
     outStream<< QVariant(data);
     return bytedata;
    }

    /// @brief Sends data of type T from  the server
    /// @param type T data to be serialised
    /// @param socket A QTcpSocket that sends the data
    /// @note Its recomended to use data can be converted to a QVariant
     template<typename T>
    static bool send(const T &data, QTcpSocket *socket)
    {
        bool sent=false;
      if (socket){
     sent=socket->write(streamOut<T>(data));
      }
     qDebug() << "TCP-SEND";
     return sent;
    }




signals:
    
public slots:
    
};

#endif // NETWORKUTILS_H
