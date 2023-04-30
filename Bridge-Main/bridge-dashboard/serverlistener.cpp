#include "serverlistener.h"
#include <QTimer>
#include <bridgeutils.h>

// Server ports
#define SERVER_UDP_PORT 5500
#define SERVER_TCP_PORT 5939
#define SERVER_LOOKUP_PORT 6060

ServerListener::ServerListener(QObject *parent) : QObject(parent) {
  discoverServer();
}

void ServerListener::discoverServer() {
  qWarning() << "discovering server at port " << SERVER_LOOKUP_PORT;
  m_serverListener = new QUdpSocket(this);
  connect(m_serverListener, &QUdpSocket::readyRead, this,
          &ServerListener::intialiseListener);
  if (!m_serverListener->bind(SERVER_LOOKUP_PORT,
                              QUdpSocket::ShareAddress |
                                  QUdpSocket::ReuseAddressHint)) {

    emit errorChanged(SERVER_NOT_FOUND,
                      "Server not found, please try again later.");
  } else {
    broadcastForServer();
  }
}

void ServerListener::broadcastForServer() {
  if (m_server_ip_address.isEmpty()) {
    BridgeUtils::broadcast("finding server ", SERVER_UDP_PORT);
    QTimer::singleShot(1000, this, &ServerListener::broadcastForServer);
  }
}

void ServerListener::intialiseListener() {

  disconnect(m_serverListener, &QUdpSocket::readyRead, nullptr, nullptr);
  QHostAddress sender;
  BridgeUtils::readBroadcast(m_serverListener, &sender, true);
  m_server_ip_address = sender.toString();
  m_serverListener->close();
  setUpServerConnections();
}

void ServerListener::setUpServerConnections() {
  m_tcpClient = new QTcpSocket(this);

  m_tcpClient->connectToHost(m_server_ip_address, SERVER_TCP_PORT,
                             QIODevice::ReadWrite);
  connect(m_tcpClient, &QTcpSocket::connected, this,
          [=]() { this->connected(); });
  connect(m_tcpClient, &QTcpSocket::readyRead, this,
          &ServerListener::dataRecieved);
  connect(m_tcpClient, &QTcpSocket::disconnected, m_tcpClient,
          &QTcpSocket::deleteLater);
  connect(m_tcpClient, &QTcpSocket::disconnected, this,
          [=]() { this->disconnected(); });
}

void ServerListener::dataRecieved() {
  qDebug() << "im lsikeefeff";
  QVariant dataAsVariant = BridgeUtils::receive(m_tcpClient);

  switch (dataAsVariant.type()) {

  // handle only map data
  case QVariant::Map: {
    handleData(dataAsVariant.toMap());
    break;
  }
  default: {}
  }
}

void ServerListener::handleData(const QVariantMap &dataAsMap) {

  qDebug() << "Handles data " << dataAsMap;

  if (dataAsMap.contains("tournament_uptime")) {
    auto uptime = dataAsMap.value("tournament_uptime").toString();
    emit tournamentUptimeChanged(uptime);

  } else if (dataAsMap.contains("type")) {

    auto dataType = dataAsMap.value("type").toString();
    auto player = dataAsMap.value("player").toInt();

    if (dataType == "new_connection") {
      emit playerConnected(player);
    } else if (dataType == "move") {
      playerMoved((player + 1) > 4 ? 0 : player);
    }
  }
}
