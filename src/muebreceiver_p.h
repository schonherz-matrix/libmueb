#ifndef LIBMUEB_MUEBRECEIVER_P_H_
#define LIBMUEB_MUEBRECEIVER_P_H_

#include <QNetworkDatagram>
#include <QUdpSocket>

#include "configuration.h"
#include "muebreceiver.h"

class MuebReceiverPrivate {
  Q_DECLARE_PUBLIC(MuebReceiver)
  Q_DISABLE_COPY(MuebReceiverPrivate)

 public:
  explicit MuebReceiverPrivate(MuebReceiver *q)
      : frame(configuration.frame()), q_ptr(q) {
    socket.bind(configuration.broadcast_animation_port());

    QObject::connect(&socket, &QUdpSocket::readyRead, q,
                     &MuebReceiver::readPendingDatagrams);

    qInfo() << "[MuebReceiver] UDP Socket will receive packets on port"
            << configuration.broadcast_animation_port();
  }

  Configuration configuration;
  libmueb::Frame frame;
  QUdpSocket socket;
  MuebReceiver *q_ptr;
};

#endif  // LIBMUEB_MUEBRECEIVER_P_H_
