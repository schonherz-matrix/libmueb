#ifndef LIBMUEB_MUEBWEBSOCKETSERVER_H_
#define LIBMUEB_MUEBWEBSOCKETSERVER_H_

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>

#include "muebtransmitter.h"

namespace libmueb {

class MuebWebSocketServer : public QObject {
  Q_OBJECT

 public:
  explicit MuebWebSocketServer(QObject* parent = nullptr);

 private slots:
  void OnNewConnection();
  void ProcessBinaryMessage(QByteArray message);
  void SocketDisconnected();

 private:
  QWebSocketServer web_socket_server_;
  QList<QWebSocket*> clients_;
  MuebTransmitter& transmitter_;
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBWEBSOCKETSERVER_H_
