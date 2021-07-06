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
        (configuration_.target_address().protocol() ==
         QAbstractSocket::IPv4Protocol)
            ? QHostAddress::AnyIPv4
            : QHostAddress::AnyIPv6,
        configuration_.animation_port(),
        QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint);

    if (configuration_.target_address().isMulticast()) {
      socket_.joinMulticastGroup(configuration_.target_address(),
                                 configuration_.multicast_interface());
    }

    QObject::connect(&socket_, &QUdpSocket::readyRead, receiver,
                     &MuebReceiver::ReadPendingDatagrams);

    qInfo().noquote()
        << QString("[MuebReceiver] UDP Socket will receive packets on port %2")
               .arg(configuration_.animation_port());
  }

  Configuration &configuration_;
  libmueb::Frame frame_;
  QUdpSocket socket_;
  MuebReceiver *q_ptr;
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBRECEIVER_P_H_
