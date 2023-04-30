#include "roommanager.h"
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QUrl>
#include <bridgeutils.h>

#include <QEventLoop>
#include <QTimer>

#define SERVER_TCP_PORT 5939
#define SERVER_UDP_PORT 5500
#define SERVER_LOOKUP_PORT 6060

RoomManager::RoomManager(QObject *parent)
    : QObject(parent), clientSocket(nullptr) {}

void RoomManager::setClientSocket(QTcpSocket *value) {
  if (value) {
    clientSocket = value;
    emit connected();
  }
}

void RoomManager::handleMessage(const QVariantMap &map) {
  qDebug() << "My map is  " << map;
  if (!map.isEmpty()) {

    auto typeAsInt = map.find("type").value().toInt();
    int table = map.find("table").value().toInt();
    int player = map.find("player").value().toInt();

    switch (static_cast<Bridge::PeerType>(typeAsInt)) {
    case Bridge::OFFER: {
      qDebug() << "creatinf and offer tos  ";
      auto offer = map.find("offer").value();

      qDebug() << "My offer is  " << offer;

      emit offerReceived(table, player, offer.toMap());
      break;
    }
    }
  }
}

void RoomManager::handleAnswer(const QVariantMap &answer, const int table,
                               const int player) {
  qDebug() << "My answer is " << answer << "  " << player << "  " << table;

  QVariantMap data = QVariantMap();

  data["table"] = table;
  data["player"] = player;
  data["type"] = static_cast<int>(Bridge::ANSWER);
  data["data"] = answer;

  if (!BridgeUtils::send(data, clientSocket)) {
    qWarning() << "Failed to send answer ";
  } else {
    qDebug() << "is sent";
  }
}

void RoomManager::peerConnectionReady(int playerSide) {
  emit peerInitalisationComplete(playerSide);
}

QVariantMap RoomManager::javascriptData() const { return m_javascriptData; }

void RoomManager::setJavascriptData(const QVariantMap &javasriptData) {
  if (javasriptData != m_javascriptData) {
    qDebug() << "setting js data ";
    m_javascriptData = javasriptData;
    emit javascriptDataChanged();
  }
}

void RoomManager::invokeJsCallBack(int peerType, const QVariantMap data) {
  qDebug() << "calling ja reaid " << peerType << "  " << data;
  emit jsInvokeRequest(peerType, data);
}
