#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "xmlengine.h"
#include <QObject>
#include <QQuickItem>
#include <QQuickView>
#include <QTimer>
#include <QVariant>
#include <roommanager.h>

class InfoDialog;
class Talker;

class GameEngine : public QObject {
  Q_OBJECT
public:
  explicit GameEngine(QObject *parent = 0);

  // Code added by Wasswa Geoffrey
  /// @brief Starts the game engine
  void start();

signals:
  void ready();

public slots:
  void reset();
  void close();
  void playMatch(QString match);
  void playListedMoves();
  void playMove(int player, int type, QString data);
  void addMessageChat(QString type, QString message, QString sender);

private:
  InfoDialog *m_dlg;
  Talker *m_talker;
  QQuickView *qmlView;
  QQuickItem *qmlItem;
  QList<StringMap> m_moveItems;

  QTimer m_playTimer;
};

#endif // GAMEENGINE_H
