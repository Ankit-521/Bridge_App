#include "bridgeutils.h"
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QQmlApplicationEngine>
#include <QtCore>
#include <QtNetwork>
#include <QtWidgets>

#define UNAUTHORISED_DECRYPTION "bAX5"
#define FILE_LOCATION "/data"

BridgeUtils::BridgeUtils() {}
void BridgeUtils::set_up_quick_widget(QQuickWidget *qquickWidget,
                                      const QUrl url, bool registerSingletons) {
  if (qquickWidget) {
    if (registerSingletons) {
      // Register constants
      qmlRegisterSingletonType(
          QUrl(
              QStringLiteral("qrc:/common_assets/qml/constants/Constants.qml")),
          "Bridge.Constants", 1, 0, "Constants");

      // Register Icons
      qmlRegisterSingletonType(
          QUrl(QStringLiteral(
              "qrc:/common_assets/qml/constants/GoogleIcons.qml")),
          "Bridge.Constants", 1, 0, "Icons");
    }

    if (!url.isEmpty())
      qquickWidget->setSource(url);

    qquickWidget->setAttribute(Qt::WA_TranslucentBackground);
    qquickWidget->setWindowFlags(Qt::FramelessWindowHint);
  } else {
    qWarning() << "Cannot set attributes on a null QQuick Widget "
               << qquickWidget;
  }
}

void reportError(QString err, QString data = QString()) {
  if (data.isEmpty())
    qDebug() << "Error receiving: " << err;
  else
    qDebug() << "Error sending: " << data << ": " << err;
}

QByteArray BridgeUtils::wrapString(QString message) {
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);

  // Code changed by Wasswa Geoffrey
  // out << message;

  out << QVariant(message);
  return block;
}

void BridgeUtils::broadcast(QString msg, int port) {
  QUdpSocket socket;
  if (socket.writeDatagram(wrapString(msg), QHostAddress::Broadcast, port) ==
      -1) {
    reportError(socket.errorString());
    return;
  }
  socket.close();
  qDebug() << "UDP-SEND:" << msg;
}

QString BridgeUtils::readBroadcast(QUdpSocket *sock, QHostAddress *senderPtr,
                                   bool readFromLocal) {
  if (!sock)
    return QString();
  QHostAddress sender;
  QString data;

  while (sock->hasPendingDatagrams()) {
    QByteArray datagram;
    datagram.resize(sock->pendingDatagramSize());
    if (sock->readDatagram(datagram.data(), datagram.size(), &sender) == -1) {
      reportError(sock->errorString());
      continue;
    }
    if (!QNetworkInterface::allAddresses().contains(sender) || readFromLocal) {
      QDataStream in(&datagram, QIODevice::ReadOnly);
      // Code changed by Wasswa Geoffrey
      // QString msg;

      QVariant msg;
      in >> msg;
      data += msg.toString();
      qDebug() << "UDP-RECV:" << msg;
    }
  }
  if (senderPtr)
    *senderPtr = sender;
  return data;
}

void BridgeUtils::tcpSend(QString msg, QTcpSocket *tcpSocket) {
  if (tcpSocket) {
    if (tcpSocket->write(wrapString(msg)) == -1) {
      reportError(tcpSocket->errorString(), msg);
      return;
    }
    qDebug() << "TCP-SEND:" << msg;
  } else {
    qWarning() << "Failed to send tcp message, QTcpsocket is null "
               << tcpSocket;
  }
}

QString BridgeUtils::tcpRecv(QTcpSocket *sock) {
  QDataStream in(sock);

  // Code changed by Wasswa Geoffrey
  // QString msg;
  QVariant msg;

  in >> msg;
  qDebug() << "TCP-RECV:" << msg.toString();
  return msg.toString();
}

void BridgeUtils::multicast(const QVariantMap &msg,
                            QList<QTcpSocket *> receiverSockets, int except) {
  for (int i = 0; i < receiverSockets.size(); i++) {
    if (i == except || !receiverSockets[i])
      continue;
    send<QVariantMap>(msg, receiverSockets[i]);
  }
}

QVariant BridgeUtils::receive(QTcpSocket *socket) {

  QVariant m_data;
  if (socket) {
    QDataStream in(socket);
    in >> m_data;
  }
  return m_data;
}

QVariantMap BridgeUtils::readData(const QString &filename) {
  QVariantMap targetMap = QVariantMap();
  if (!filename.isEmpty()) {
    const QString filePath = makeDirAndGetPath(FILE_LOCATION, false);

    if (!filePath.isEmpty()) {

      QFile file(filePath + "/" + filename);
      if (file.open(QIODevice::ReadOnly)) {
        auto data = QByteArray::fromBase64(file.readAll());
        targetMap = QJsonDocument::fromJson(data).toVariant().toMap();
      }
    }
  }

  return targetMap;
}

bool BridgeUtils::storeData(const QVariantMap &data, const QString filename) {
  bool bStored = false;
  if (!data.isEmpty() && !filename.isEmpty()) {
    const QString filepath = makeDirAndGetPath(FILE_LOCATION);
    QJsonDocument doc = QJsonDocument::fromVariant(data);

    if (!filepath.isEmpty() && !doc.isEmpty()) {
      QFile file(filepath + "/" + filename);

      // remove file if exits
      if (file.exists()) {
        file.remove();
      }
      if (file.open(QFile::WriteOnly)) {
        // prevent unauthorised hacking
        // Simple security :)
        bStored =
            file.write(doc.toJson().toBase64(QByteArray::KeepTrailingEquals));
        file.close();
      }
    }

  } else {
    qWarning() << "Cannot store " << data << " on file " << filename;
  }
  return bStored;
}

QString
BridgeUtils::makeDirAndGetPath(const QString &folder, bool createFolder,
                               QStandardPaths::StandardLocation location) {

  QString mDataPath = "";
  const QString appDataPath = QStandardPaths::writableLocation(location);
  QDir appDataDir(appDataPath);
  if (!appDataDir.exists() && createFolder)
    appDataDir.mkdir(appDataPath);
  if (appDataDir.exists()) {

    mDataPath = appDataPath + folder;

    QDir dir(mDataPath);

    if (!dir.exists()) {
      if (!dir.mkdir(mDataPath)) {
        mDataPath = "";
      }
    }
  }
  return mDataPath;
}

QByteArray BridgeUtils::readData(const QString &filename,
                                 const QString &filePath) {
  QByteArray targetMap;
  if (!filename.isEmpty() && !filePath.isEmpty()) {

    QFile file(filePath + "/" + filename);
    qDebug() << "is my sthene " << file.exists() << "  "
             << (filePath + "/" + filename);
    if (file.open(QIODevice::ReadOnly)) {
      targetMap = file.readAll();
    }
  }

  return targetMap;
}

QString BridgeUtils::polishTime(const int time) {
  QString targetTime = QString::number(time);
  if (time <= 9) {
    targetTime = "0" + QString::number(time);
  }

  return targetTime;
}
