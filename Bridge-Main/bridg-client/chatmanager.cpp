#include "chatmanager.h"
#include "networkutils.h"
#include "xmlengine.h"

ChatManager::ChatManager(QTcpSocket *tcpClient, QObject *parent) : QObject(parent),  m_tcpClient{tcpClient}{}

void ChatManager::sendMessagePartner(const QString &message)
{
    StringMap mapClient;
    mapClient["message"] = "chat";
    mapClient.insert("type", "partner");
    mapClient.insert("chat", message);
    NetworkUtils::tcpSend(XMLEngine::buildXml(mapClient), m_tcpClient);
}

void ChatManager::sendMessageTable(const QString &message)
{
    StringMap mapClient;
    mapClient["message"] = "chat";
    mapClient.insert("type", "table");
    mapClient.insert("chat", message);
    NetworkUtils::tcpSend(XMLEngine::buildXml(mapClient), m_tcpClient);
}

void ChatManager::sendMessageGlobal(const QString &message)
{
    StringMap mapClient;
    mapClient["message"] = "chat";
    mapClient.insert("type", "global");
    mapClient.insert("chat", message);
    NetworkUtils::tcpSend(XMLEngine::buildXml(mapClient), m_tcpClient);
}

void ChatManager::sendMessageServer(const QString &message)
{
    StringMap mapClient;
    mapClient["message"] = "chat";
    mapClient.insert("type", "server");
    mapClient.insert("chat", message);
    NetworkUtils::tcpSend(XMLEngine::buildXml(mapClient), m_tcpClient);
}
