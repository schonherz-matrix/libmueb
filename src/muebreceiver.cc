#include "muebreceiver.h"

#include <cstring>

#include "muebreceiver_p.h"

namespace libmueb {

MuebReceiver::MuebReceiver(QObject *parent)
    : QObject(parent), d_ptr_(new MuebReceiverPrivate(this)) {}

MuebReceiver::~MuebReceiver() { delete d_ptr_; }

MuebReceiver &MuebReceiver::Instance() {
  static MuebReceiver instance;

  return instance;
}

QPixmap MuebReceiver::frame() const {
  Q_D(const MuebReceiver);

  return QPixmap::fromImage(d->frame_);
}

namespace {

inline void datagram_uncompress_error() {
  qWarning() << "[MuebReceiver] Processed packet is invalid! Check the header "
                "or packet contents(size)";
}

}  // namespace

// Process datagram
void MuebReceiver::ReadPendingDatagrams() {
  Q_D(MuebReceiver);

  while (d->socket_.hasPendingDatagrams()) {
    if (d->socket_.pendingDatagramSize() == d->configuration_.packet_size()) {
      const QNetworkDatagram &datagram = d->socket_.receiveDatagram();
      QByteArray data = datagram.data();

      // Packet header check
      // Check protocol
      if (data.at(0) != d->configuration_.protocol_type()) {
        datagram_uncompress_error();
        return;
      }

      // Check packet number
      auto packet_number = data.at(1);
      if (packet_number >= d->configuration_.max_packet_number() ||
          packet_number < 0) {
        datagram_uncompress_error();
        return;
      }

      data.remove(0, d->configuration_.packet_header_size());
      auto frame_begin =
          d->frame_.bits() +
          packet_number * d->configuration_.frame_fragment_size();

      // Uncompress 1 byte into 2 color components
      if (d->configuration_.color_depth() < 5) {
        for (auto &i : data) {
          *frame_begin = i & 0xf0;
          frame_begin++;
          *frame_begin = (i & 0x0f) << d->configuration_.factor();
          frame_begin++;
        }
        // No compression
      } else {
        std::memcpy(frame_begin, data.constData(), data.size());
      }

      emit FrameChanged(QPixmap::fromImage(d->frame_));
    }
    // Drop invalid packet
    else {
      qWarning() << "[MuebReceiver] Packet has invalid size!"
                 << d->socket_.pendingDatagramSize() << "bytes";

      d->socket_.receiveDatagram(0);
    }
  }
}

}  // namespace libmueb
