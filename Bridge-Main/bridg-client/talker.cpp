#include "talker.h"
#include "networkutils.h"
#include "xmlengine.h"
#include <QMessageBox>
#include <QRegExp>
#include <QStringList>
#include <QTimer>

#include "bridgeutils.h"

#define SERVER_UDP_PORT 5500
#define SERVER_TCP_PORT 5939
#define SERVER_LOOKUP_PORT 6060

#include <roommanager.h>

Talker::Talker(QObject *parent)
    : QObject(parent), clientRoomManager(new RoomManager) {
  m_tcpClient = 0;
}

void Talker::discoverServer() {
  m_servListener = new QUdpSocket(this);
  connect(m_servListener, SIGNAL(readyRead()), this, SLOT(saveServerAddress()));
  if (!m_servListener->bind(SERVER_LOOKUP_PORT,
                            QUdpSocket::ShareAddress |
                                QUdpSocket::ReuseAddressHint)) {
    QMessageBox::information(NULL, "Can't find the server",
                             "Cannot find the server, please close and start "
                             "this application again.");
  } else {
    broadcastForServer();
  }
}

void Talker::broadcastForServer() {
  if (!m_server.isEmpty())
    return;
  NetworkUtils::broadcast("discover", SERVER_UDP_PORT);
  QTimer::singleShot(1000, this, SLOT(broadcastForServer()));
}

void Talker::saveServerAddress() {
  disconnect(m_servListener, SIGNAL(readyRead()), 0, 0);
  QHostAddress sender;
  NetworkUtils::readBroadcast(m_servListener, &sender, true);
  m_server = sender.toString();
  m_servListener->close();
  connectToServer();
}

void Talker::connectToServer() {
  //    m_tcpClient = new QTcpSocket(); // Commented by Rajiv
  m_tcpClient = new QTcpSocket(this); // Added by Rajiv
  m_chatManager = new ChatManager(m_tcpClient, this);

  clientRoomManager->setClientSocket(m_tcpClient);

  m_tcpClient->connectToHost(m_server, SERVER_TCP_PORT, QIODevice::ReadWrite);
  connect(m_tcpClient, SIGNAL(connected()), this, SIGNAL(serverConnected()));
  connect(m_tcpClient, SIGNAL(readyRead()), this, SLOT(readServer()));
  connect(m_tcpClient, SIGNAL(disconnected()), m_tcpClient,
          SLOT(deleteLater()));
  connect(m_tcpClient, SIGNAL(disconnected()), this, SLOT(emitfromserver()));
}

void Talker::identify(int table, int player) {
  m_table = table;
  m_player = player;
  m_map.clear();
  m_map.insert("message", "identify");
  m_map.insert("table", QString::number(table));
  m_map.insert("player", QString::number(player));
  NetworkUtils::tcpSend(XMLEngine::buildXml(m_map), m_tcpClient);
}
void Talker::emitfromserver() { qDebug() << "disconnected from server"; }
void Talker::readServer() {
  // Code changed by Wasswa Geoffrey

  // QString msg = NetworkUtils::tcpRecv(m_tcpClient);
  // parseMessage(msg);

  QVariant dataAsVariant = NetworkUtils::receive(m_tcpClient);

  // qDebug() << "My data ys  " << dataAsVariant.type();
  switch (dataAsVariant.type()) {
  case QVariant::String: {
    qDebug() << "My data ys  " << dataAsVariant.toString();
    parseMessage(dataAsVariant.toString());
    break;
  }
  case QVariant::Map: {
    // qDebug() << "Im in map ";
    auto dataAsMap = dataAsVariant.toMap();
    if (dataAsMap.contains("tournament_uptime")) {
      auto uptime = dataAsMap.value("tournament_uptime").toString();
      emit tournamentUptimeChanged(uptime);

    } else if (dataAsMap.contains("type")) {
    } else {

      clientRoomManager->handleMessage(dataAsVariant.toMap());
    }
    break;
  }
  default: {}
  }
  //  QVariantMap dataAsMap=NetworkUtils::receive<QVariantMap>(m_tcpClient);

  //    else {

  //    }
}

void Talker::parseMessage(QString msg) {
  const StringMap stuff = XMLEngine::parseXml(msg);

  QString message = stuff["message"].toLower();

  if (message == "ack") {
    int ok = stuff["ok"].toInt();
    emit authenticated(ok);
    if (ok) {
      emit newGame();
      if (m_player > 3 && stuff.contains("data")) {
        emit matchReceived(stuff["data"]);
      }
    }
  } else if (message == "board") {
    emit boardReceived(stuff["dealer"].toInt(), stuff["vulnerable"].toInt(),
                       m_table, stuff["round"].toInt(), stuff["number"].toInt(),
                       stuff["ns_pair"].toInt(), stuff["ew_pair"].toInt());
    // used for sending dummy cards
    m_cards = stuff["cards"];
    QStringList cardsList = m_cards.split(QChar(','));
    emit cardsReceived(m_player, QVariant::fromValue(cardsList));
  } else if (message == "move") {
    emit moveReceived(stuff["player"].toInt(), stuff["type"].toInt(),
                      stuff["data"]);
    // ankit alert
    // receiving alert message
    if (stuff["messagealert"] != "") {
      emit alertnotificationReceived(stuff["messagealert"]);
      // ankit
    }
  }
  // ankit claim
  else if (message == "claim") {
    emit claimReceived(stuff["claim_num"].toInt(), stuff["msg"],
                       stuff["player"].toInt());
  } else if (message == "claimresponse") {
    qDebug() << "claimresponse from server";
    qDebug() << stuff["claim_num"];
    emit claimresponseReceived(stuff["response"], stuff["claim_num"].toInt(),
                               stuff["playerTo"].toInt());
  }
  // ankit
  else if (message == "dummy_cards") {
    if (m_player > 3)
      return;
    QStringList dummyCards = stuff["cards"].split(QChar(','));
    emit dummyCardsReceived(QVariant::fromValue(dummyCards));
  } else if (message == "notification") {
    int pair_number = stuff["pair_number"].toInt();

    QStringList pointsList;
    for (int i = 0; i < pair_number; i++) {
      pointsList.insert(i, stuff["pair_" + QString::number(i)]);
    }
    emit notificationReceived(stuff["note"], QVariant::fromValue(pointsList));

  } else if (message == "resume_data") {
    emit matchReceived(stuff["data"]);
  } else if (message == "chat") {
    emit messageChatReceived(stuff["type"], stuff["chat"], stuff["player"]);
  } else {
    qDebug() << "is me "
             << "currently running ";
  }
}

RoomManager *Talker::getClientRoomManager() const { return clientRoomManager; }

int Talker::player() { return m_player; }

void Talker::setCards(QString cards) { m_cards = cards; }

void Talker::broadcastMove(int type, QString data) {
  StringMap m = m_map;
  m["message"] = "move";
  m.insert("type", QString::number(type));
  m.insert("data", data);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
  qDebug() << "sending move " << data;
}

// ankit similar to broadcastmove
void Talker::broadcastAlert(int type, QString data, QString data2) {
  StringMap m = m_map;
  m["message"] = "alert";
  m.insert("type", QString::number(type));
  m.insert("data", data);
  m.insert("messagealert", data2);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}
// ankit
// ankit similar to broadcastmove
void Talker::broadcastClaim(int type, int claim_num, QString claim_msg) {
  StringMap m = m_map;
  m["message"] = "claim";
  m.insert("type", QString::number(type));
  m.insert("claim_num", QString::number(claim_num));
  m.insert("msg", claim_msg);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}
void Talker::broadcastClaimResponse(int type, int player2, QString response,
                                    int claim_num) {
  StringMap m = m_map;

  m["message"] = "claimresponse";
  m["response"] = response;
  m.insert("claim_num", QString::number(claim_num));
  m.insert("type", QString::number(type));
  m.insert("playerTo", QString::number(player2));
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}
// ankit

void Talker::broadcastDummyCards() {
  StringMap m = m_map;
  m["message"] = "dummy_cards";
  m.insert("cards", m_cards);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}

void Talker::broadcastMessageChat(QString type, QString message) {
  if (type == "global") {
    m_chatManager->sendMessageGlobal(message);
  } else if (type == "table") {
    m_chatManager->sendMessageTable(message);
  } else if (type == "partner") {
    m_chatManager->sendMessagePartner(message);
  } else if (type == "server") {
    m_chatManager->sendMessageServer(message);
  }
}

void Talker::reportScore(int score) {
  StringMap m = m_map;
  m["message"] = "score";
  m.insert("score", QString::number(score));
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}

void Talker::sendReady() {

  emit playerNumber(m_player);
  if (m_player > 3)
    return;

  StringMap m = m_map;
  m["message"] = "ready";

  // Code chnaged by Wasswa Geoffrey
  // NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
  qDebug() << "sending ---------- ";
  BridgeUtils::tcpSend(XMLEngine::buildXml(m), m_tcpClient);
}
