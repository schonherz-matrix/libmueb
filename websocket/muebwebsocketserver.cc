#include "muebwebsocketserver.h"

#include <QDebug>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

namespace libmueb {

static QString GetIdentifier(QWebSocket *peer) {
  return QStringLiteral("%1:%2").arg(peer->peerAddress().toString(),
                                     QString::number(peer->peerPort()));
}

MuebWebSocketServer::MuebWebSocketServer(QObject *parent)
    : QObject(parent),
      web_socket_server_(QStringLiteral("libmueb websocket server"),
                         QWebSocketServer::NonSecureMode),
      transmitter_(MuebTransmitter::Instance()) {
  if (web_socket_server_.listen(QHostAddress::Any, 50003)) {
    qInfo() << "Server listening on port" << web_socket_server_.serverPort();

    connect(&web_socket_server_, &QWebSocketServer::newConnection, this,
            &MuebWebSocketServer::OnNewConnection);
  }
}

void MuebWebSocketServer::OnNewConnection() {
  auto socket = web_socket_server_.nextPendingConnection();

  qInfo() << GetIdentifier(socket) << "connected!";

  socket->setParent(this);

  connect(socket, &QWebSocket::binaryMessageReceived, this,
          &MuebWebSocketServer::ProcessBinaryMessage);
  connect(socket, &QWebSocket::disconnected, this,
          &MuebWebSocketServer::SocketDisconnected);

  clients_ << socket;

  // Send configuration
  QJsonObject root;
  root["width"] = QJsonValue::fromVariant(transmitter_.width());
  root["height"] = QJsonValue::fromVariant(transmitter_.height());
  root["pixels"] = QJsonValue::fromVariant(transmitter_.pixels());
  root["frame_size_in_bytes"] = transmitter_.FrameSizeInBytes();

  QJsonDocument doc(root);
  socket->sendTextMessage(
      QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
}

void MuebWebSocketServer::ProcessBinaryMessage(QByteArray message) {
  for (QWebSocket *client : qAsConst(clients_)) {
    // Do not echo message back to sender
    if (client != sender()) {
      client->sendBinaryMessage(message);
    }
  }

  transmitter_.SendFrame(QImage(
      reinterpret_cast<const uchar *>(message.constData()),
      transmitter_.width(), transmitter_.height(), QImage::Format_RGBA8888));
}

void MuebWebSocketServer::SocketDisconnected() {
  auto client = qobject_cast<QWebSocket *>(sender());

  qInfo() << GetIdentifier(client) << "disconnected!";

  if (client) {
    clients_.removeAll(client);
    client->deleteLater();
  }
}

}  // namespace libmueb
