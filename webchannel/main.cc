#include <QApplication>
#include <QWebChannel>
#include <QWebSocketServer>

#include "muebchannel.h"
#include "shared/websocketclientwrapper.h"
#include "shared/websockettransport.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // setup the QWebSocketServer
  QWebSocketServer server(QStringLiteral("QWebChannel libmueb Server"),
                          QWebSocketServer::NonSecureMode);
  if (!server.listen(QHostAddress::LocalHost, 12345)) {
    qFatal("Failed to open web socket server.");
    return 1;
  }

  // wrap WebSocket clients in QWebChannelAbstractTransport objects
  WebSocketClientWrapper clientWrapper(&server);

  // setup the channel
  QWebChannel channel;
  QObject::connect(&clientWrapper, &WebSocketClientWrapper::clientConnected,
                   &channel, &QWebChannel::connectTo);

  // setup the MuebChannel and publish it to the QWebChannel
  libmueb::MuebChannel mueb_channel;
  channel.registerObject(QStringLiteral("mueb_channel"), &mueb_channel);

  return a.exec();
}
