#ifndef LIBMUEB_MUEBTRANSMITTER_P_H_
#define LIBMUEB_MUEBTRANSMITTER_P_H_

#include <QByteArray>
#include <QDebug>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QtConcurrent>
#include <functional>

#include "configuration.h"
#include "muebtransmitter.h"

namespace libmueb {

class MuebTransmitterPrivate {
  Q_DECLARE_PUBLIC(MuebTransmitter)
  Q_DISABLE_COPY(MuebTransmitterPrivate)

 public:
  explicit MuebTransmitterPrivate(MuebTransmitter* transmitter)
      : configuration_(Configuration::Instance()),
        datagram_(QByteArray(), configuration_.target_address(),
                  configuration_.animation_port()),
        q_ptr(transmitter) {
    qInfo().noquote()
        << QString("[MuebTransmitter] UDP Socket will send frame to %2:%3")
               .arg(configuration_.target_address().toString())
               .arg(configuration_.animation_port());
  }

  Configuration& configuration_;
  QUdpSocket socket_;
  QNetworkDatagram datagram_;
  MuebTransmitter* q_ptr;
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBTRANSMITTER_P_H_
