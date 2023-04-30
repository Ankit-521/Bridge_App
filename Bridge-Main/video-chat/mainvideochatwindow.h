#ifndef MAINVIDEOCHATWINDOW_H
#define MAINVIDEOCHATWINDOW_H

#include <QMainWindow>

#include <QJsonValue>
#include <video-chat_global.h>

#include <QHash>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class VIDEOCHAT_EXPORT MainVideoChatWindow : public QMainWindow {

  /// @brief typedef of QHash<int,int>
  typedef QHash<int, int> PlayerInfo;

  Q_PROPERTY(int tables MEMBER m_tables CONSTANT NOTIFY tablesChanged)

  Q_OBJECT
public:
  /// @brief Stores player position, table, and tcpsocket to the room
  static void playerConnected(int table, int player, QTcpSocket *socket);

  enum PossibleJsError { CAMERA_ERROR };
  Q_ENUM(PossibleJsError)

  /// @brief Intialises the web engine module used in the Video Chat window
  static void initialiseWindow();

  void eventStopped();
  MainVideoChatWindow(QWidget *parent = nullptr);
  ~MainVideoChatWindow();

  /// @brief handles request recieved from the server
  static void newRequest(const QVariantMap &request);

  /// @brief Intialises the video and shows the window. Its recommended to use
  /// this function instead of QMainWindow::show().
  void showWindow();

  /// @brief Sets the total number of tables available in the tournament
  void setTables(int tableSize);

  /// @brief Invites a player from table to join the tournament room
  Q_INVOKABLE void invitePlayer(int player, int table, const QVariant &data);

signals:
  void tablesChanged();
  void eventCancelled();
  void newPlayerJoined(int table, int playerPosition, bool joined);
  void errorChanged(PossibleJsError error, const QString &errorMsg);
  void send(const QVariantMap &data, QTcpSocket *playerSocket);
  void jsInvokeRequest(int peertype, const QVariantMap &data, int player = -1,
                       int table = -1);

public slots:

  /// @brief Adds joined players to the table in the Room
  void playerJoined(int tableId, int playerId, bool joined);
  void error(int errorNumber, const QJsonValue &errorMsg);

protected:
  void storePlayer(int table, int player, QTcpSocket *socket);
  void handleRequest(const QVariantMap &request);

  /// @brief Helper function that to pass c++ data to javascript
  void invokeJsCallBack(int peerType, const QVariantMap data);

private:
  Ui::MainWindow *ui;
  int m_tables;

  // Inner map is for player and his table
  QHash<PlayerInfo, QTcpSocket *> players;
};

#endif // MAINVIDEOCHATWINDOW_H
