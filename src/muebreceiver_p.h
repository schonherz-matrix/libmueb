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
      : configuration_(Configuration::Instance()),
        frame_(configuration_.frame()),
        q_ptr(receiver) {
    socket_.bind(
        configuration_.animation_port(),
        QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);

    QObject::connect(&socket_, &QUdpSocket::readyRead, receiver,
                     &MuebReceiver::ReadPendingDatagrams);
    qInfo().noquote()
        << QString(
               "[MuebReceiver(%1)] UDP Socket will receive packets on port %2")
               .arg(configuration_.debug_mode() ? "DEBUG MODE" : "NORMAL MODE")
               .arg(configuration_.animation_port());
  }

  Configuration &configuration_;
  libmueb::Frame frame_;
  QUdpSocket socket_;
  MuebReceiver *q_ptr;
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBRECEIVER_P_H_
