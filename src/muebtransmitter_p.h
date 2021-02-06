#ifndef LIBMUEB_MUEBTRANSMITTER_P_H_
#define LIBMUEB_MUEBTRANSMITTER_P_H_

#include <QByteArray>
#include <QDebug>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QtConcurrent>

#include "configuration.h"
#include "muebtransmitter.h"

class MuebTransmitterPrivate {
  Q_DECLARE_PUBLIC(MuebTransmitter)
  Q_DISABLE_COPY(MuebTransmitterPrivate)

 public:
  explicit MuebTransmitterPrivate(MuebTransmitter* transmitter)
      : datagram_(QByteArray(), configuration_.target_address(),
                  configuration_.broadcast_animation_port()),
        q_ptr(transmitter) {
    qInfo().noquote() << "[MuebTransmitter] UDP Socket will send frame to"
                      << QString("%1:%2")
                             .arg(configuration_.target_address().toString())
                             .arg(configuration_.broadcast_animation_port());
  }

  Configuration configuration_;
  QUdpSocket socket_;
  QNetworkDatagram datagram_;
  MuebTransmitter* q_ptr;
};

#endif  // LIBMUEB_MUEBTRANSMITTER_P_H_
