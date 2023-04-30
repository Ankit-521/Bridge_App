#ifndef BRIDGEUTILS_H
#define BRIDGEUTILS_H

#include "Bridge-Utils_global.h"
#include <QDir>
#include <QHostAddress>
#include <QStandardPaths>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QVariantMap>
#include <QtQuickWidgets/QQuickWidget>

/// @brief An enum that contains miscellaneous enums used throughout the
/// Bridge app
namespace Bridge {
enum PeerType { OFFER, ANSWER, CANDIDATE };
}

/// @brief Helper class that contains miscellaneous functions used throughout
/// the Bridge app
class BRIDGEUTILS_EXPORT BridgeUtils {
public:
  BridgeUtils();

  /// @brief Helper function to set QQuick Widget attributes and source url
  /// @param qquickWidget The QQuickWidget on which the attributes should be set
  /// @param url The QUrl object with the qml file to load
  /// @param registerSingletons If true, the app qml singletons will be
  /// registered
  static void set_up_quick_widget(QQuickWidget *qquickWidget,
                                  const QUrl url = QUrl(),
                                  bool registerSingletons = true);

  static QByteArray wrapString(QString msg);
  static void broadcast(QString msg, int port);
  static QString readBroadcast(QUdpSocket *sock, QHostAddress *senderPtr = 0,
                               bool readFromLocal = false);

  /// @brief Sends a tcpSocket message to the server
  /// @param msg Message to be sent
  /// @param tcpSocket A QTcpSocket that is used to send.
  static void tcpSend(QString msg, QTcpSocket *tcpSocket);

  static QString tcpRecv(QTcpSocket *sock);

  /// Sends a message msg to all players in the tournament
  static void multicast(const QVariantMap &msg, QList<QTcpSocket *> receivers,
                        int except = -1);

  // code added by Wasswa Geoffrey

  /// @brief Helper function to serialise data of type T for sending from the
  /// server
  /// @param type T data to be serialised
  /// @note Its recomended to use data can be converted to a QVariant
  template <typename T> static QByteArray streamOut(const T &data) {
    QByteArray bytedata = QByteArray();
    QDataStream outStream(&bytedata, QIODevice::WriteOnly);
    outStream << QVariant(data);
    return bytedata;
  }

  /// @brief Sends data of type T from  the server
  /// @param type T data to be serialised
  /// @param socket A QTcpSocket that sends the data
  /// @note Its recomended to use data can be converted to a QVariant
  template <typename T> static bool send(const T &data, QTcpSocket *socket) {
    bool sent = false;
    if (socket) {
      sent = socket->write(streamOut<T>(data));
    }
    qDebug() << "TCP-SEND";
    return sent;
  }

  /// @brief Recieves data from  the server
  /// @param socket A QTcpSocket that sent the data
  /// @note Its recomended to use data can be converted to a QVariant
  static QVariant receive(QTcpSocket *socket);

  /// @brief Reads data on filename from QStandard::AppDataLocation directory
  static QVariantMap readData(const QString &filename);

  /// @brief Creates a file with filename and  stores data on it the
  /// QStandard::AppDataLocation directory
  static bool storeData(const QVariantMap &data, const QString filename);

  /// @brief Creates a directory at location with a folder and returns the path,
  /// pass in false not to create the folder
  static QString makeDirAndGetPath(const QString &folder = "",
                                   bool createFolder = true,
                                   QStandardPaths::StandardLocation location =
                                       QStandardPaths::AppDataLocation);
  /// @brief Opens a file with fileName on a path filePath and returns its
  /// content as a QByteArray
  static QByteArray readData(const QString &filename, const QString &filePath);

  /// @brief Polishes time to a standard time output
  static QString polishTime(const int time);
};

#endif // BRIDGEUTILS_H
