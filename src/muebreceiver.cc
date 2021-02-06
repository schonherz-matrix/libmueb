#include "muebreceiver.h"

#include <cstring>

#include "muebreceiver_p.h"

MuebReceiver::MuebReceiver(QObject *parent)
    : QObject(parent), d_ptr_(new MuebReceiverPrivate(this)) {}

MuebReceiver::~MuebReceiver() { delete d_ptr_; }

MuebReceiver &MuebReceiver::Instance() {
  static MuebReceiver instance;

  return instance;
}

libmueb::Frame MuebReceiver::frame() { return d_ptr_->frame; }

inline void datagram_uncompress_error() {
  qWarning() << "[MuebReceiver] Processed packet is invalid! Check the header "
                "or packet contents(size)";
}

void MuebReceiver::ReadPendingDatagrams() {
  Q_D(MuebReceiver);

  while (d->socket.hasPendingDatagrams()) {
    if (d->socket.pendingDatagramSize() == d->configuration.packet_size()) {
      QNetworkDatagram datagram = d->socket.receiveDatagram();
      QByteArray data = datagram.data();

      // Process datagram
      // Packet header check
      // Check protocol
      if (data[0] != d->configuration.protocol_type()) {
        datagram_uncompress_error();
        return;
      }

      auto packet_number = data[1];
      if (packet_number >= d->configuration.max_packet_number() ||
          packet_number < 0) {
        datagram_uncompress_error();
        return;
      }

      data.remove(0, d->configuration.packet_header_size());
      auto frame_begin = d->frame.bits() +
                         packet_number * d->configuration.frame_fragment_size();

      // Uncompress 1 byte into 2 color components
      if (d->configuration.color_depth() < 5) {
        for (auto i : data) {
          *frame_begin = i & 0xf0;
          frame_begin++;
          *frame_begin = (i & 0x0f) << d->configuration.factor();
          frame_begin++;
        }
        // No compression
      } else {
        std::memcpy(frame_begin, data.constData(), data.size());
      }

      emit(FrameChanged(d->frame));
    }
    // Drop invalid packet
    else {
      qWarning() << "[MuebReceiver] Packet has invalid size!"
                 << d->socket.pendingDatagramSize() << "bytes";

      d->socket.receiveDatagram(0);
    }
  }
}
