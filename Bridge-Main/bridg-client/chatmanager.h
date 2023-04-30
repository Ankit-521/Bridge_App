#pragma once

#include <QObject>
#include <QTcpSocket>

class ChatManager : public QObject
{
    Q_OBJECT
public:
    explicit ChatManager(QTcpSocket *tcpClient, QObject *parent);
    void sendMessagePartner(const QString &message);
    void sendMessageTable(const QString &message);
    void sendMessageGlobal(const QString &message);
    void sendMessageServer(const QString &message);

private :
    QTcpSocket *m_tcpClient {nullptr};
};
