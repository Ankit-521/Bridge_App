#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer> //Rajiv

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
    
    // code added by Wasswa Geoffrey

    /// @brief Helper function to serialise data of type T for sending from the server
    /// @param type T data to be serialised
    /// @note Its recomended to use data can be converted to a QVariant
     template<typename T>
    static QByteArray streamOut(const T &data)
    {
     QByteArray bytedata=QByteArray();
     QDataStream outStream(&bytedata,QIODevice::WriteOnly);
     outStream<< data;
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

    /// @brief Recieves data from  the server
    /// @param socket A QTcpSocket that sent the data
    /// @note Its recomended to use data can be converted to a QVariant
    static QVariant receive(QTcpSocket *socket);


    
};

#endif // NETWORKUTILS_H
