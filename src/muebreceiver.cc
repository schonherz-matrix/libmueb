#include "muebreceiver.h"

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
  while (d_ptr_->socket.hasPendingDatagrams()) {
    if (d_ptr_->socket.pendingDatagramSize() ==
        d_ptr_->configuration.packet_size()) {
      QNetworkDatagram datagram = d_ptr_->socket.receiveDatagram();
      QByteArray data = datagram.data();

      // Process datagram
      // Packet header check
      // Check protocol
      if (data[0] != d_ptr_->configuration.protocol_type()) {
        datagram_uncompress_error();
        return;
      }

      auto packet_number = data[1];
      if (packet_number >= d_ptr_->configuration.max_packet_number() ||
          packet_number < 0) {
        datagram_uncompress_error();
        return;
      }

      data.remove(0, d_ptr_->configuration.packet_header_size());
      auto frame_begin =
          d_ptr_->frame.bits() +
          packet_number * d_ptr_->configuration.frame_fragment_size();

      // Uncompress 1 byte into 2 color components
      if (d_ptr_->configuration.color_depth() < 5) {
        for (auto i : data) {
          *frame_begin = i & 0xf0;
          frame_begin++;
          *frame_begin = (i & 0x0f) << d_ptr_->configuration.factor();
          frame_begin++;
        }
        // No compression
      } else {
        // FIXME use better copy method
        for (auto i : data) {
          *frame_begin = i;
          frame_begin++;
        }
      }

      emit(FrameChanged(d_ptr_->frame));
    }
    // Drop invalid packet
    else {
      qWarning() << "[MuebReceiver] Packet has invalid size!"
                 << d_ptr_->socket.pendingDatagramSize() << "bytes";

      d_ptr_->socket.receiveDatagram(0);
    }
  }
}
