#include "chatmanager.h"
#include "networkutils.h"
#include "xmlengine.h"
#include <QTcpSocket>

ChatManager::ChatManager(QObject *parent) : QObject(parent) {}

void ChatManager::sendMessageGlobal(const QString &message, QList<QList<QTcpSocket *>> receivers, const QString &player, int table)
{
    QString namePlayer = player + " (Table "+QString::number(table+1) + ")";
    StringMap m;

    m.insert("message", "chat");
    m.insert("type", "global");
    m.insert("chat", message);
    m.insert("player", namePlayer);

    for (auto const &receiver : receivers) {
        NetworkUtils::multicast(XMLEngine::buildXml(m), receiver);
    }
}

void ChatManager::sendMessageTable(const QString &message, QList<QTcpSocket*> receivers, const QString &player)
{
    StringMap m;
    m.insert("message", "chat");
    m.insert("type", "table");
    m.insert("chat", message);
    m.insert("player", player);
    NetworkUtils::multicast(XMLEngine::buildXml(m), receivers);
}

void ChatManager::sendMessagePartner(const QString &message, QList<QTcpSocket *> receivers, const QString &player)
{
    StringMap m;
    m.insert("message", "chat");
    m.insert("type", "partner");
    m.insert("chat", message);
    m.insert("player", player);
    NetworkUtils::multicast(XMLEngine::buildXml(m), receivers);
}

void ChatManager::sendMessageFromServerToGlobal(const QString &message, QList<QList<QTcpSocket *>> receivers)
{
    StringMap m;
    m.insert("message", "chat");
    m.insert("type", "global");
    m.insert("chat", message);
    m.insert("player", "Server");

    for (auto const &receiver : receivers) {
        NetworkUtils::multicast(XMLEngine::buildXml(m), receiver);
    }}

void ChatManager::sendMessageFromServerToTables(const QString &message, QList<QTcpSocket *> receivers)
{
    StringMap m;
    m.insert("message", "chat");
    m.insert("type", "table");
    m.insert("chat", message);
    m.insert("player", "Server");

    NetworkUtils::multicast(XMLEngine::buildXml(m), receivers);
}

void ChatManager::sendMessageFromServerToPartner(const QString &message, QList<QTcpSocket *> receivers)
{
    StringMap m;
    m.insert("message", "chat");
    m.insert("type", "partner");
    m.insert("chat", message);
    m.insert("player", "Server");

    NetworkUtils::multicast(XMLEngine::buildXml(m), receivers);
}
