#include "gameengine.h"
#include "infodialog.h"
#include "talker.h"

#include <QDomDocument>
#include <QGraphicsObject>
#include <QQmlContext>
#include <QQuickView>

#include <roommanager.h>

void GameEngine::start() {
  m_dlg->show();
  m_talker->discoverServer();
}

GameEngine::GameEngine(QObject *parent) : QObject(parent) {

  m_dlg = new InfoDialog();

  m_talker = new Talker(this);

  // dlg and talker
  connect(m_talker, SIGNAL(serverConnected()), m_dlg,
          SLOT(updateLabelServerConnected()));
  connect(m_dlg, SIGNAL(authenticateRequest(int, int)), m_talker,
          SLOT(identify(int, int)));
  connect(m_talker, SIGNAL(authenticated(bool)), m_dlg,
          SLOT(updateLabelAuthenticated(bool)));

  // talker and this
  connect(m_talker, SIGNAL(matchReceived(QString)), this,
          SLOT(playMatch(QString)));
  connect(m_talker, SIGNAL(moveReceived(int, int, QString)), this,
          SLOT(playMove(int, int, QString)));
  connect(this, SIGNAL(ready()), m_talker, SLOT(sendReady()));
  connect(m_talker, SIGNAL(newGame()), this, SLOT(reset()));
  connect(m_talker, &Talker::messageChatReceived, this,
          &GameEngine::addMessageChat);

  // Code changed by Wasswa Geoffrey
  // m_dlg->show();
  // m_talker->discoverServer();

  qmlView = 0;

  // reset(); // comment this
}

void GameEngine::reset() {

  if (qmlView) {
    disconnect(qmlView, 0, 0, 0);
    disconnect(qmlItem, 0, 0, 0);
    while (!qmlView->close())
      ;
    qmlView->deleteLater();
    qmlItem->deleteLater();
  }
  qmlView = new QQuickView();

  // expose the roommanager object to qml
  qmlView->rootContext()->setContextProperty("roommanager",
                                             m_talker->getClientRoomManager());

  qmlView->rootContext()->setContextProperty("talker", m_talker);
  qmlView->setSource(QUrl("qrc:/qml/main.qml"));

  // qmlView->showFullScreen(); //Commented by Rajiv
  // qmlView->showMinimized(); //New Code added by Rajiv

  qmlItem = qmlView->rootObject();
  // qmlView->setWindowFlags(Qt::Window);
  qmlView->setFlags(Qt::Window); // Rajiv

  // qmlView->setAttribute(Qt::WA_LockLandscapeOrientation, true);
  connect(qmlItem, SIGNAL(newGame()), this, SLOT(reset()));

  connect(qmlItem, SIGNAL(closed()), this, SLOT(close()));
  qmlView->setResizeMode(
      QQuickView::SizeViewToRootObject); // rechange this sizeRootObjectToView
  qmlView->show();
  // remove this line
  emit ready();
}
void GameEngine::close() {
  if (qmlView) {
    disconnect(qmlView, 0, 0, 0);
    disconnect(qmlItem, 0, 0, 0);
    while (!qmlView->close())
      ;
    qmlView->deleteLater();
    qmlItem->deleteLater();
  }
}
void GameEngine::playMatch(QString match) {
  QDomDocument doc;
  doc.setContent(match);

  QDomElement matchElem = doc.documentElement();
  QDomElement board = matchElem.elementsByTagName("board").at(0).toElement();

  qDebug() << "my boaid " << board.attribute("table");
  qDebug() << " my maths " << matchElem.attribute("table");

  QMetaObject::invokeMethod(
      qmlItem, "setBoard", Q_ARG(QVariant, board.attribute("dealer").toInt()),
      Q_ARG(QVariant, board.attribute("vulnerable").toInt()),
      Q_ARG(QVariant, m_talker->getTableNo()),
      Q_ARG(QVariant, matchElem.attribute("round").toInt() + 1),
      Q_ARG(QVariant, matchElem.attribute("boardNo").toInt() + 1),
      Q_ARG(QVariant, matchElem.attribute("ns_pair").toInt() + 1),
      Q_ARG(QVariant, matchElem.attribute("ew_pair").toInt() + 1));

  QDomNodeList allCards = board.elementsByTagName("cards");
  for (int i = 0; i < allCards.size(); i++) {
    QDomElement elem = allCards.at(i).toElement();
    int player = elem.attribute("player").toInt();
    QStringList cards = elem.text().split(QChar(','));
    if (m_talker->player() == player) {
      m_talker->setCards(elem.text());
    }
    QMetaObject::invokeMethod(qmlItem, "showCards", Q_ARG(QVariant, player),
                              Q_ARG(QVariant, cards));
  }

  QDomNodeList moves = matchElem.elementsByTagName("item");
  for (int i = 0; i < moves.size(); i++) {
    m_moveItems << XMLEngine::parseXml(moves.at(i).toElement());
  }

  QMetaObject::invokeMethod(qmlItem, "setAnimated", Q_ARG(QVariant, false));
  connect(&m_playTimer, SIGNAL(timeout()), this, SLOT(playListedMoves()));
  m_playTimer.start(50);
}

void GameEngine::playListedMoves() {
  if (m_moveItems.isEmpty()) {
    m_playTimer.stop();
    QMetaObject::invokeMethod(qmlItem, "setAnimated", Q_ARG(QVariant, true));
    return;
  }
  StringMap move = m_moveItems.takeFirst();

  if (move["message"] != "move" && move["message"] != "dummy_cards") {
    if (!m_moveItems.isEmpty())
      playListedMoves();
    return;
  }

  if (move["message"] == "move") {
    QMetaObject::invokeMethod(
        qmlItem, "showMove", Q_ARG(QVariant, move["player"].toInt()),
        Q_ARG(QVariant, move["type"].toInt()), Q_ARG(QVariant, move["data"]));
  } else {
    QStringList cards = move["cards"].split(QChar(','));
    QMetaObject::invokeMethod(qmlItem, "showDummyCards",
                              Q_ARG(QVariant, cards));
  }

  if (m_moveItems.isEmpty()) {
    m_playTimer.stop();
    QMetaObject::invokeMethod(qmlItem, "setAnimated", Q_ARG(QVariant, true));
  }
}

void GameEngine::playMove(int player, int type, QString data) {
  if (m_moveItems.isEmpty()) {
    QMetaObject::invokeMethod(qmlItem, "showMove", Q_ARG(QVariant, player),
                              Q_ARG(QVariant, type), Q_ARG(QVariant, data));
  } else {
    StringMap m;
    m.insert("message", "move");
    m.insert("player", QString::number(player));
    m.insert("type", QString::number(type));
    m.insert("data", data);
    m_moveItems << m;
  }
}

void GameEngine::addMessageChat(QString type, QString message, QString sender) {
  QMetaObject::invokeMethod(qmlItem, "addMessageChat", Q_ARG(QVariant, type),
                            Q_ARG(QVariant, message), Q_ARG(QVariant, sender));
}
