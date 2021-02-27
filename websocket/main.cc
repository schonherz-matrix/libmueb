#include <QGuiApplication>

#include "muebwebsocketserver.h"

int main(int argc, char *argv[]) {
  QGuiApplication a(argc, argv);
  libmueb::MuebWebSocketServer mueb_web_socket;

  return a.exec();
}
