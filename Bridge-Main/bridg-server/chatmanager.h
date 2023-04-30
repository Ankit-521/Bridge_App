#pragma once

#include <QObject>

class QTcpSocket;

class ChatManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatManager(QObject *parent);

public:
    void sendMessageGlobal(const QString& message, QList<QList<QTcpSocket *>>receivers, const QString &player, int table);
    void sendMessageTable(const QString& message, QList<QTcpSocket *> receivers, const QString &player);
    void sendMessagePartner(const QString& message, QList<QTcpSocket *> receivers, const QString &player);
    void sendMessageFromServerToGlobal(const QString& message, QList<QList<QTcpSocket *> > receivers);
    void sendMessageFromServerToTables(const QString& message, QList<QTcpSocket *> receivers);
    void sendMessageFromServerToPartner(const QString& message, QList<QTcpSocket *> receivers);
};
