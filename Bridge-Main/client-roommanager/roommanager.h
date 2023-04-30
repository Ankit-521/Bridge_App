#ifndef ROOMMANAGER_H
#define ROOMMANAGER_H

#include <QDebug>
#include <QObject>

#include "client-roommanager_global.h"
#include <QTcpSocket>

class CLIENTROOMMANAGER_EXPORT RoomManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariantMap javascriptData READ javascriptData WRITE
                 setJavascriptData NOTIFY javascriptDataChanged)

public:
  explicit RoomManager(QObject *parent = nullptr);

  /// @brief sets the client TCP socket
  void setClientSocket(QTcpSocket *value);

  /// @brief Handles incomming data from the server
  void handleMessage(const QVariantMap &map);

  QVariantMap javascriptData() const;
  void setJavascriptData(const QVariantMap &javascriptData);

  /// @brief This function aids in passing of c++ data to javascript
  Q_INVOKABLE void invokeJsCallBack(int peerType, const QVariantMap data);

public slots:
  void callMeFromJs(const QVariant &v) { qDebug() << "My rum  ss " << v; }

  /// @brief Handles the client Answer. This Function is called from Javascript
  void handleAnswer(const QVariantMap &answer, const int table,
                    const int player);

  /// @brief This function is called from Javascript when a peer connection
  /// intialisation is done.
  void peerConnectionReady(int playerSide);

  QVariantMap callMe() {
    qDebug() << "my js data us " << javascriptData();
    return javascriptData();
  }

signals:
  void connected();
  void offerReceived(const int table, const int playerPosition,
                     const QVariantMap offer);
  void peerInitalisationComplete(int playerSide);
  void javascriptDataChanged();
  void jsInvokeRequest(int peer, const QVariantMap &data);

protected:
  inline bool isClientSocketValid() { return (clientSocket); }

private:
  QTcpSocket *clientSocket;

  // Use this variable to expose data to javasript
  QVariantMap m_javascriptData;
};

#endif // ROOMMANAGER_H
