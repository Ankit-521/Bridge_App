#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H
#include <QObject>
#include <QTcpSocket>
#include <QUdpSocket>

/// @brief A listener class that listens for incoming messages/data from the
/// server
class ServerListener : public QObject {
  Q_OBJECT
public:
  explicit ServerListener(QObject *parent = nullptr);

  enum ErrorCodes {
    NONE,
    SERVER_NOT_FOUND

  };

signals:
  void errorChanged(ErrorCodes code, const QString &errorString);
  void connected();
  void disconnected();
  void tournamentUptimeChanged(const QString &uptime);
  void playerConnected(int player);
  void playerMoved(int player);

protected:
  /// @brief Searches and connects to the server
  void discoverServer();

  /// @brief Handles data sent from the server.
  void handleData(const QVariantMap &map);
protected slots:

  /// @brief Broadcasts small messages  to the server
  void broadcastForServer();

  /// @brief Intialises this listner when a server is found
  void intialiseListener();

  /// @brief Connects this listner to the server
  void setUpServerConnections();

  /// @brief Reads any data sent from the server
  void dataRecieved();

private:
  QString m_server_ip_address;  // server's IP address
  QUdpSocket *m_serverListener; // for discovering the server
  QTcpSocket *m_tcpClient;      // for all in-game communication with server
};

#endif // SERVERLISTENER_H
