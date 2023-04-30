#ifndef SERVER_H
#define SERVER_H

#include "board.h"
#include <QList>
#include <QQueue>
#include <QTcpServer>
#include <QTimer>
#include <QUdpSocket>
class MovementModel;
class XMLEngine;
class ChatManager;

class Server : public QObject {
  Q_OBJECT
public:
  explicit Server(MovementModel *model, QObject *parent = 0);
  inline QList<Board> boards() { return m_boards; }
  void sendMatchToSpec(QString data, int table);
  void sendMatchToPlayer(QString data, int table, int player);
  ~Server();

signals:
  void clientConnected(int, int, bool);
  void tableReady(int table, int player);
  void moveReceived(int table, QString move);
  void scoreReceived(int table, int score);
  void specArrived(int table);
  void incomingMessageFromClient(const QString message, const int table,
                                 const QString sender);

  // code added by wasswa geoffrey
  void onNewConnection();

public slots:
  void readDatagrams();
  void connectToClient();
  void readClient();
  void prepareTable(int table);
  void startGame();
  void notifyTableChange(int, int, int, int, int, int, int);
  void notifyTournamentEnd(int table);
  void emitClientDisconnected();
  void sendMessageFromServer(const QString &message,
                             const QVector<int> tables = QVector<int>(),
                             const QString &playerPair = 0) const;
  void sendVideoMessage(const QVariantMap &data, QTcpSocket *playerSocket);

  /// Code added by Wasswa Geoffrey
  /// @brief Starts timer for this tournament
  inline void startTimer() {
    // trigger every second
    m_tournament_timer->start(1000);
  }

  /// @brief Stops timer for this tournament
  void stopTimer();

protected:
  /// @brief Resets the tournament period
  void resetTournament();

  /// @brief Sends the current state of the tournament to all players
  void multicastUptime(const QString &state);

  /// Sends a message to all connected sockets
  void mulitcast(const QVariantMap &data);

protected slots:
  /// @brief Broadcasts the period of the tournament to all players
  void broadCastTournamentUptime();

  /// @brief Removes a TCP socket from the list of connected TCP sockets
  void removeSocket(QTcpSocket *socket);

private:
  void parseClient(QString msg, QTcpSocket *sender);
  void multicastMove(int table, int player, QString message);
  void notify(QString notification, QTcpSocket *receiver);
  void closeConnections(int table);
  QString getPlayerType(const int table, const QTcpSocket *sender)
      const; // Type player: "North", "South", "East", "West"
  unsigned int getTablePlayerBySocket(
      const QTcpSocket *sender) const; // Get the the table player with socket
  QTcpSocket *getPartnerBySocket(const int table, QTcpSocket *sender)
      const; // Get the partner player with socket

  QUdpSocket *m_udpListener;         // listener for incoming broadcast
  QTcpServer *m_tcpListener;         // listener for incoming connections
  QList<QTcpSocket *> m_authClients; // list of authenticated clients
  QList<QList<QTcpSocket *>>
      m_tableSockets; // list <table, list<player, socket>>
  QList<QList<QTcpSocket *>> m_waitingSpecs; // list <table, list<spec, socket>>
  QList<Board> m_boards;                     // list of boards
  QList<int> m_tableReadyCount;              // list <table, readyCount>
  QQueue<int> m_tableQ; // queue of tables ready to begin the game
  MovementModel *m_model;
  ChatManager *m_chatManager{nullptr};

  // Code added by wasswa Geoffrey
  QTimer *m_tournament_timer;
  int minutes;
  int hours;
  int seconds;
  int days;
  int months;
  int years;

  // store connected sockets
  QList<QTcpSocket *> connectedSockets;
};

#endif // SERVER_H
