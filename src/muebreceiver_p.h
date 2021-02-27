#ifndef LIBMUEB_MUEBRECEIVER_P_H_
#define LIBMUEB_MUEBRECEIVER_P_H_

#include <QNetworkDatagram>
#include <QUdpSocket>

#include "configuration.h"
#include "muebreceiver.h"

namespace libmueb {

class MuebReceiverPrivate {
  Q_DECLARE_PUBLIC(MuebReceiver)
  Q_DISABLE_COPY(MuebReceiverPrivate)

 public:
  explicit MuebReceiverPrivate(MuebReceiver *receiver)
      : configuration(Configuration::Instance()),
        frame(configuration.frame()),
        q_ptr(receiver) {
    socket.bind(
        configuration.broadcast_animation_port(),
        QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);

    QObject::connect(&socket, &QUdpSocket::readyRead, receiver,
                     &MuebReceiver::ReadPendingDatagrams);
    qInfo().noquote()
        << QString(
               "[MuebReceiver(%1)] UDP Socket will receive packets on port %2")
               .arg(configuration.debug_mode() ? "DEBUG MODE" : "NORMAL MODE")
               .arg(configuration.broadcast_animation_port());
  }

  Configuration &configuration;
  libmueb::Frame frame;
  QUdpSocket socket;
  MuebReceiver *q_ptr;
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBRECEIVER_P_H_
