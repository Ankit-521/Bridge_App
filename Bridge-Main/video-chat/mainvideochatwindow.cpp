
#include "mainvideochatwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QQmlContext>
#include <QUrl>
#include <QtWebEngine/QtWebEngine>
#include <bridgeutils.h>
#include <utils.h>

MainVideoChatWindow *mWindow = nullptr;

void MainVideoChatWindow::playerConnected(int table, int player,
                                          QTcpSocket *socket) {
  qDebug() << "a plaer is conenec r " << table << "  " << player << "  "
           << socket;
  if (socket && mWindow) {
    mWindow->storePlayer(table, player, socket);
  }
}

void MainVideoChatWindow::initialiseWindow() {
  QtWebEngine::initialize(); // Essential for web engine to work in QML
}

void MainVideoChatWindow::eventStopped() {
  qDebug() << "Event is ended ";

  emit eventCancelled();
  setTables(0);
}

MainVideoChatWindow::MainVideoChatWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_tables(0) {
  ui->setupUi(this);
  mWindow = this;
}

MainVideoChatWindow::~MainVideoChatWindow() { delete ui; }

void MainVideoChatWindow::newRequest(const QVariantMap &request) {
  if (!request.isEmpty() && mWindow) {
    mWindow->handleRequest(request);
  } else {
    qDebug() << "Failed to execute request";
  }
}

void MainVideoChatWindow::showWindow() {
  static bool bInitialised = false; // prevent unnecessary initialisations
  if (!bInitialised) {
    // expose this class to qml
    ui->quickWidget->rootContext()->setContextProperty("mainWindow", this);
    set_up_quick_widget(ui->quickWidget,
                        QUrl(QStringLiteral("qrc:/qml/main.qml")));
  }
  bInitialised = true;
  show();
}

void MainVideoChatWindow::handleRequest(const QVariantMap &request) {
  qDebug() << " a message is received  " << request;
  // Ensure a valid request
  if (request.contains("type")) {

    int type = request.find("type").value().toInt();
    qDebug() << "my typ is  " << type;

    switch (static_cast<Bridge::PeerType>(type)) {
    case Bridge::ANSWER: {
      auto data = request.find("data").value().toMap();
      int player = request.find("player").value().toInt();
      int table = request.find("table").value().toInt();
      qDebug() << "Data us  " << data;
      if (!data.isEmpty()) {
        emit jsInvokeRequest(type, data, player, table);
      }
      break;
    }
    }
  }
}

void MainVideoChatWindow::invokeJsCallBack(int peerType,
                                           const QVariantMap data) {
  qDebug() << "calling ja reaid " << peerType << "  " << data;
  emit jsInvokeRequest(peerType, data);
}

void MainVideoChatWindow::setTables(int tableSize) {
  if (tableSize != m_tables) {
    m_tables = tableSize;
    emit tablesChanged();
  }
}

void MainVideoChatWindow::invitePlayer(int player, int table,
                                       const QVariant &offerdata) {

  // create an offer
  QVariantMap data = QVariantMap();

  data["player"] = player;
  data["table"] = table;
  data["type"] = static_cast<int>(Bridge::PeerType::OFFER);
  data["offer"] = offerdata;

  PlayerInfo playerinfo;
  playerinfo[table] = player;
  auto playerSocket = players.find(playerinfo).value();

  if (playerSocket) {
    // send offer to player
    emit send(data, playerSocket);
  } else {
    qWarning() << "cannot find player socket";
  }
}

void MainVideoChatWindow::playerJoined(int tableId, int playerId, bool joined) {

  emit newPlayerJoined(tableId, playerId, joined);
}

void MainVideoChatWindow::error(int errorNumber, const QJsonValue &errorMsg) {

  emit errorChanged(static_cast<PossibleJsError>(errorNumber),
                    errorMsg.toString());
}

void MainVideoChatWindow::storePlayer(int table, int player,
                                      QTcpSocket *socket) {
  QHash<int, int> playerInfo;
  playerInfo[table] = player;
  players[playerInfo] = socket;
}
