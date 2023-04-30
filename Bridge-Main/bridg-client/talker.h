#ifndef TALKER_H
#define TALKER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>

#include <chatmanager.h>
#include <roommanager.h>

class Talker : public QObject {
  Q_OBJECT
public:
  explicit Talker(QObject *parent = 0);
  void discoverServer();
  Q_INVOKABLE void broadcastMove(int type, QString data);
  // ankit
  Q_INVOKABLE void broadcastAlert(int type, QString data, QString data2);
  // ankit alert
  // ankit
  Q_INVOKABLE void broadcastClaim(int type, int claim_num, QString claim_msg);
  // ankit claim
  // ankit
  Q_INVOKABLE void broadcastClaimResponse(int type, int player2,
                                          QString response, int claim_num);
  // ankit claimaccept
  Q_INVOKABLE void broadcastDummyCards();
  Q_INVOKABLE void broadcastMessageChat(QString type, QString message);
  Q_INVOKABLE void reportScore(int score);

  int player();
  void setCards(QString cards);
  inline int getTableNo(){return m_table; };//ankit

  /// Code added by Wasswa Geoffrey

  RoomManager *getClientRoomManager() const;

signals:
  void serverConnected();
  void authenticated(bool);
  void newGame();
  void playerNumber(int num);
  void boardReceived(int dealer, int vulnerable, int table, int round,
                     int boardNo, int ns_pair, int ew_pair);
  void cardsReceived(int player, QVariant cards);
  void moveReceived(int player, int type, QString data);
  void dummyCardsReceived(QVariant cards);
  void notificationReceived(QString note, QVariant pointsList);
  // ankit notifying alert
  void alertnotificationReceived(QString alert);
  // ankit
  void claimReceived(int claim_num, QString claim_msg,
                     int claim_player); // claim
  void claimresponseReceived(QString response, int num_tricks,
                             int playerTo); // claim accept
  void matchReceived(QString log);
  void messageChatReceived(QString type, QString message, QString player);

  // Code added by Wasswa Geoffrey
  void tournamentUptimeChanged(const QString &uptime);

public slots:
  void broadcastForServer();
  void saveServerAddress();
  void identify(int table, int player);
  void readServer();
  void sendReady();
  void emitfromserver();

private:
  int m_table;
  int m_player;
  QMap<QString, QString> m_map;
  QString m_cards;

  void connectToServer();
  void parseMessage(QString msg);

  ChatManager *m_chatManager{nullptr};
  RoomManager *clientRoomManager;
  QString m_server;           // server's IP address
  QUdpSocket *m_servListener; // for discovering the server
  QTcpSocket *m_tcpClient;    // for all in-game communication with server
};

#endif // TALKER_H
