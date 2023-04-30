#include "server.h"
#include "chatmanager.h"
#include "movementmodel.h"
#include "networkutils.h"
#include "xmlengine.h"
#include <QMessageBox>
#include <QRegExp>
#include <QTime>
#include <QTimer>
#include <bridgeutils.h>

#define UDP_PORT 5500
#define TCP_PORT 5939
#define CLIENT_PORT 6060

#include <mainvideochatwindow.h>
Server::Server(MovementModel *model, QObject *parent)
    : QObject(parent), m_model(model) {
  // UDP listener
  m_udpListener = new QUdpSocket(this);
  while (!m_udpListener->bind(UDP_PORT, QUdpSocket::DontShareAddress))
    ;
  connect(m_udpListener, SIGNAL(readyRead()), this, SLOT(readDatagrams()));

  // TCP listener
  m_tcpListener = new QTcpServer(this);
  if (!m_tcpListener->listen(QHostAddress::Any, TCP_PORT)) {
    QMessageBox::critical(
        NULL, "Network error",
        "Another instance of server seems to be running already.");
  } else {
    connect(m_tcpListener, SIGNAL(newConnection()), this,
            SLOT(connectToClient()));
  }
  qDebug() << "Server up";

  // set up tables
  for (int i = 0; i < m_model->tables(); i++) {
    QList<QTcpSocket *> t; // m_tablesoket[tableno][4];
    t << 0 << 0 << 0 << 0;
    m_tableSockets << t;
    m_waitingSpecs << QList<QTcpSocket *>();
    m_tableReadyCount << 0;
  }

  // set up boards
  qsrand((uint)QTime::currentTime().msec());
  for (int i = 0; i < m_model->totalBoards(); i++) {
    m_boards << Board(i);
  }

  // Create Chat Manager
  m_chatManager = new ChatManager(this);

  // Code added by wasswa Geoffrey
  m_tournament_timer = new QTimer();
  connect(m_tournament_timer, &QTimer::timeout, this,
          &Server::broadCastTournamentUptime);

  resetTournament();
}

Server::~Server() {
  m_tcpListener->close();
  for (int i = 0; i < m_model->tables(); i++)
    closeConnections(i);
}

void Server::readDatagrams() {
  NetworkUtils::readBroadcast(m_udpListener, 0, true);
  NetworkUtils::broadcast("I am here", CLIENT_PORT);

  qDebug() << "im conenec eerefevevevev";
}

void Server::connectToClient() {

  qDebug() << "New connecteio arrows";
  QTcpSocket *s = m_tcpListener->nextPendingConnection();
  connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));

  connect(s, SIGNAL(disconnected()), this, SLOT(emitClientDisconnected()));

  // Code changed by Wasswa Geoffrey
  connect(s, &QTcpSocket::disconnected, this, [=]() { removeSocket(s); });

  emit onNewConnection();

  // Code added by wasswa Geoffrey
  connectedSockets << s;
}

void Server::readClient() {

  // Code changed by Wasswa Geoffrey

  // QTcpSocket *s = qobject_cast<QTcpSocket *>(sender());
  // QString msg = NetworkUtils::tcpRecv(s);

  // parseClient(msg, s);

  QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
  QVariant data = BridgeUtils::receive(socket);

  qDebug() << "My data is " << data << "  " << data.type();

  switch (data.type()) {
  case QVariant::String: {
    parseClient(data.toString(), socket);
    break;
  }
  case QVariant::Map: {
    qDebug() << "is a map";
    auto dataAsMap = data.toMap();

    MainVideoChatWindow::newRequest(data.toMap());
    break;
  }
  default: {}
  }
}

void Server::mulitcast(const QVariantMap &data) {

  BridgeUtils::multicast(data, connectedSockets);
}

void Server::parseClient(QString msg, QTcpSocket *sender) {

  const StringMap stuff = XMLEngine::parseXml(msg);
  QString message = stuff["message"];
  int table = stuff["table"].toInt() - 1;
  int player = stuff["player"].toInt();

  if (message != "identify" && !m_authClients.contains(sender))
    return;
  if (message == "identify") {
    // Insert the player in the table
    int ok = (table >= 0 && table < m_tableSockets.size() &&
              m_model->roundBoard(table).first < m_model->rounds());
    if (ok) {
      if (player < 4) {
        if (m_tableSockets[table][player] == 0) {
          m_tableSockets[table][player] = sender;
          m_authClients << sender;
        } else {
          ok = 0;
        }
      } else { // spectator
        if (m_tableReadyCount[table] < 4)
          m_waitingSpecs[table] << sender;
        else
          m_tableSockets[table] << sender;
      }
    }

    if (player < 4 || !ok) {
      StringMap m;
      m.insert("message", "ack");
      m.insert("ok", QString::number(ok));
      NetworkUtils::tcpSend(XMLEngine::buildXml(m), sender);
    }

    if (ok) {
      if (player < 4) {
        emit clientConnected(table, player, true);

        // code added by Wasswa geoffrey
        // store this player
        MainVideoChatWindow::playerConnected(table, player, sender);

        /// Multicast new player connect
        QVariantMap data;
        data["player"] = player;
        data["type"] = "new_connection";
        mulitcast(data);

      } else if (m_tableReadyCount[table] >= 4)
        emit specArrived(table);
    }
  } else if (message == "ready") {
    if (++m_tableReadyCount[table] >= 4) {
      emit tableReady(table, player);
    }
  } else if (message ==
             "alert") { // ankit alert//when server receive alert it sends alert
                        // to opponents and no alert to partner
    StringMap m = stuff;
    m["message"] = "move";
    QString mess = (XMLEngine::buildXml(m));
    NetworkUtils::tcpSend(mess, m_tableSockets[table][(player + 1) % 4]);
    NetworkUtils::tcpSend(mess, m_tableSockets[table][(player + 3) % 4]);
    m["messagealert"] = "";
    QString mess2 = (XMLEngine::buildXml(m));
    NetworkUtils::tcpSend(mess2, m_tableSockets[table][(player + 2) % 4]);
    // multicastMove(table, player, mess);
    emit moveReceived(table, mess);
  } // ankit
  else if (message == "move" || message == "dummy_cards") {
    multicastMove(table, player, msg);
    emit moveReceived(table, msg);
    QVariantMap data;
    data["player"] = player;
    data["type"] = "move";
    mulitcast(data);

  } else if (message == "claim") { // ankit claim//when server receive claim it
                                   // sends claim to opponents
    qDebug() << "claim message received";
    NetworkUtils::tcpSend(msg, m_tableSockets[table][(player + 1) % 4]);
    NetworkUtils::tcpSend(msg, m_tableSockets[table][(player + 3) % 4]);
    // emit moveReceived(table, msg);
  } else if (message == "claimresponse") {
    qDebug() << "claim response message received";
    if (stuff["response"] == "accepted") {
      for (int i = 0; i < 4; i++) {
        NetworkUtils::tcpSend(msg, m_tableSockets[table][i]);
      }
    } else {
      NetworkUtils::tcpSend(msg,
                            m_tableSockets[table][stuff["playerTo"].toInt()]);
      NetworkUtils::tcpSend(msg, m_tableSockets[table][(player + 2) % 4]);
    }

  }
  // ankit
  else if (message == "score") {
    int score = stuff["score"].toInt();
    emit scoreReceived(table, score);
    m_tableReadyCount[table] = 0;
  } else if (message == "chat") {
    QString message = stuff["chat"];
    QString type = stuff["type"];

    int table = getTablePlayerBySocket(sender);
    qDebug() << type;
    if (type == "global") {
      m_chatManager->sendMessageGlobal(message, m_tableSockets,
                                       getPlayerType(table, sender), table);
    } else if (type == "table") {
      m_chatManager->sendMessageTable(message, m_tableSockets[table],
                                      getPlayerType(table, sender));
    } else if (type == "partner") {
      QTcpSocket *receiver = getPartnerBySocket(table, sender);
      QList<QTcpSocket *> receivers;
      receivers.append(sender);
      receivers.append(receiver);
      m_chatManager->sendMessagePartner(message, receivers,
                                        getPlayerType(table, sender));
    } else if (type == "server") {
      qDebug("sss");
      emit incomingMessageFromClient(message, table,
                                     getPlayerType(table, sender));
    }
  }
}

void Server::prepareTable(int table) {
  m_tableQ.enqueue(table);
  QTimer::singleShot(1000, this, SLOT(startGame()));
}

void Server::startGame() {
  // prepare dealer, vulnerable team
  int table = m_tableQ.dequeue();
  Board flap = m_boards[m_model->boardNo(table)];
  int round = m_model->roundBoard(table).first;
  QPair<int, int> pairs = m_model->pairsOnTable(round, table);
  StringMap m;
  m.insert("message", "board");
  m.insert("number", QString::number(m_model->boardNo(table) + 1));
  m.insert("dealer", QString::number(flap.dealer()));
  m.insert("vulnerable", QString::number(flap.vulnerableTeam()));
  m.insert("round", QString::number(round + 1));
  m.insert("ns_pair", QString::number(pairs.first + 1));
  m.insert("ew_pair", QString::number(pairs.second + 1));

  // prepare cards and send board+cards
  QList<QTcpSocket *> sockets = m_tableSockets[table];
  for (int j = 0; j < 4; j++) {
    QString cards;
    foreach (const int &card, flap.cardsOf(j)) {
      cards += QString::number(card) + ",";
    }
    cards = cards.left(cards.size() - 1); // remove the trailing comma
    m["cards"] = cards;
    NetworkUtils::tcpSend(XMLEngine::buildXml(m), sockets.at(j));
  }

  // send match to specs
  int size = m_waitingSpecs[table].size();
  for (int j = 0; j < size; j++) {
    m_tableSockets[table] << m_waitingSpecs[table].takeFirst();
    emit specArrived(table);
  }
}

void Server::multicastMove(int table, int player, QString msg) {
  QList<QTcpSocket *> sockets = m_tableSockets[table];
  NetworkUtils::multicast(msg, sockets, player);
}
void Server::notify(QString notification, QTcpSocket *receiver) {
  if (!receiver)
    return;
  StringMap m;
  m.insert("message", "notification");
  m.insert("note", notification);
  QMap<int, QString>::iterator i;
  int pair_count = 0;
  for (i = m_model->result_board.begin(); i != m_model->result_board.end();
       ++i) {
    pair_count++;
    m.insert("pair_" + QString::number(i.key()), i.value());
  }
  m.insert("pair_number", QString::number(pair_count));
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), receiver);
}
QString nextTableNotification(int table, int team, int nextHere) {
  QString note;
  if (table == -1) {
    note = "Sit out for next round";
  } else {
    note = QString("Go next to Table-%1 %2")
               .arg(table + 1)
               .arg(team ? " EW" : " NS");
  }
  note.append(QString("\nPair %1 will play here for next round").arg(nextHere));
  return note;
}

/* @params:
table: table number who will receive the notification message
ns_table: table number where NS will sit for next round
ns_team: 0 if NS will sit as NS for next round, 1 otherwise
ew_table: table number where EW will sit for next round
ew_team: 0 if EW will sit as NS for next round, 1 otherwise
next_ns: pair number that will sit as NS at this table for next round
next_ew: pair number that will sit as EW at this table for next round
*/
void Server::notifyTableChange(int table, int ns_table, int ns_team,
                               int ew_table, int ew_team, int next_ns,
                               int next_ew) {
  QList<QTcpSocket *> sockets = m_tableSockets[table];
  if (ns_table != table) {
    QString ns_notification = nextTableNotification(ns_table, ns_team, next_ns);
    notify(ns_notification, sockets[0]);
    notify(ns_notification, sockets[2]);
  }
  if (ew_table != table) {
    QString ew_notification = nextTableNotification(ew_table, ew_team, next_ew);
    notify(ew_notification, sockets[1]);
    notify(ew_notification, sockets[3]);
  }
}

void Server::notifyTournamentEnd(int table) {
  foreach (QTcpSocket *s, m_tableSockets[table].mid(0, 4)) {
    notify("Tournament over.\nResults will be declared soon.\nThank you for "
           "participating in the tournament.",
           s);
  }
  closeConnections(table);
  qDebug() << "tournament is ended" << table;
}

void Server::closeConnections(int table) {
  for (int i = 0; i < m_tableSockets[table].size(); i++) {
    if (m_tableSockets[table][i] == 0)
      continue;
    m_tableSockets[table][i]->close();
    m_tableSockets[table][i] = 0;
    emit clientConnected(table, i, false);
  }
  for (int i = 0; i < m_waitingSpecs[table].size(); i++) {
    if (m_waitingSpecs[table][i] == 0)
      continue;
    m_waitingSpecs[table][i]->close();
    m_waitingSpecs[table][i] = 0;
  }
}

QString Server::getPlayerType(const int table, const QTcpSocket *sender) const {
  for (auto i = 0; i < m_tableSockets[table].size(); ++i) {
    if (m_tableSockets[table][i] == sender) {
      if (i == 0)
        return "North";
      else if (i == 1)
        return "East";
      else if (i == 2)
        return "South";
      else if (i == 3)
        return "West";
      else if (i == 4)
        return "Spectator";
    }
  }

  return QString{};
}

unsigned int Server::getTablePlayerBySocket(const QTcpSocket *sender) const {
  unsigned int indexTable{0};

  for (const auto &table : m_tableSockets) {
    for (const auto &inner : table) {
      if (sender == inner) {
        return indexTable;
      }
    }
    indexTable++;
  }

  return indexTable;
}

QTcpSocket *Server::getPartnerBySocket(const int table,
                                       QTcpSocket *sender) const {
  QTcpSocket *partner{nullptr};

  auto index = m_tableSockets[table].indexOf(sender);

  // 0: North
  // 1: East
  // 2: South
  // 3: West
  // Partner: NS and EW

  if (index == 0) {
    partner = m_tableSockets[table][2];
  } else if (index == 1) {
    partner = m_tableSockets[table][3];
  } else if (index == 2) {
    partner = m_tableSockets[table][0];
  } else if (index == 3) {
    partner = m_tableSockets[table][1];
  }

  return partner;
}

void Server::emitClientDisconnected() {
  QTcpSocket *s = qobject_cast<QTcpSocket *>(sender());
  int table, player = -1;
  for (table = 0; table < m_tableSockets.size() &&
                  (player = m_tableSockets[table].indexOf(s)) == -1;
       table++)
    ;
  if (player == -1)
    return;
  qDebug() << "disconnected:"
           << "table" << table << "player" << player;
  m_tableSockets[table][player] = 0;
  m_authClients.removeAll(s);
  m_tableReadyCount[table]--;
  if (player < 4)
    emit clientConnected(table, player, false);
}

void Server::sendMessageFromServer(const QString &message,
                                   const QVector<int> tables,
                                   const QString &playerPair) const {
  // Send To Global
  if (tables.size() == 0) {
    m_chatManager->sendMessageFromServerToGlobal(message, m_tableSockets);
  }

  // Send To Tables
  else if (tables.size() > 0 && playerPair.isEmpty()) {
    QList<QTcpSocket *> sockets;

    for (auto const table : tables) {
      sockets.append(m_tableSockets[table]);
    }

    m_chatManager->sendMessageFromServerToTables(message, sockets);
  }

  // Send To player Pair
  // playerPair = "NS"
  // playerPair = "EW"
  else {
    QList<QTcpSocket *> sockets;

    for (auto const table : tables) {
      if (playerPair == "NS") {
        sockets.append(m_tableSockets[table][0]);
        sockets.append(m_tableSockets[table][2]);

      } else {
        sockets.append(m_tableSockets[table][1]);
        sockets.append(m_tableSockets[table][3]);
      }
    }
    m_chatManager->sendMessageFromServerToPartner(message, sockets);
  }
}

void Server::sendVideoMessage(const QVariantMap &data,
                              QTcpSocket *playerSocket) {
  qDebug() << "sending dat a " << data << "  " << playerSocket;

  if (!NetworkUtils::send<QVariantMap>(data, playerSocket)) {
    qDebug() << "Failed to send data"
             << "something is wrong ";
  } else {
    qDebug() << "data is sent";
  }
}

void Server::stopTimer() {
  multicastUptime("Tournament stopped.");
  m_tournament_timer->stop();
}

void Server::sendMatchToSpec(QString data, int table) {
  StringMap m;
  m.insert("message", "ack");
  m.insert("ok", "1");
  m.insert("data", data);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tableSockets[table].last());
}

void Server::sendMatchToPlayer(QString data, int table, int player) {
  StringMap m;
  m.insert("message", "resume_data");
  m.insert("data", data);
  NetworkUtils::tcpSend(XMLEngine::buildXml(m), m_tableSockets[table][player]);
}

void Server::resetTournament() {
  minutes = 0;
  hours = 0;
  seconds = 0;
  years = 0;
  days = 0;
  months = 0;
}

void Server::multicastUptime(const QString &state) {

  QVariantMap data;
  data["tournament_uptime"] = state;

  BridgeUtils::multicast(data, connectedSockets);
}

void Server::broadCastTournamentUptime() {

  // 1 minute
  if (seconds == 60) {

    seconds = 0;
    minutes++;
    if (minutes == 60) {
      hours++;
      minutes = 0;
      if (hours == 24) {
        hours = 0;
        days++;
      }
    }
  }

  QString time;
  time = BridgeUtils::polishTime(hours) + ":" +
         BridgeUtils::polishTime(minutes) + ":" +
         BridgeUtils::polishTime(seconds);

  multicastUptime(time);
  seconds++;
}

void Server::removeSocket(QTcpSocket *socket) {
  if (socket) {
    if (!connectedSockets.removeOne(socket)) {
      qWarning() << __LINE__ << __FUNCTION__ << "Failed to remove socket";
    }
  }
}
