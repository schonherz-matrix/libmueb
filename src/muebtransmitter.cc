#include "muebtransmitter.h"

#include "muebtransmitter_p.h"

MuebTransmitter::MuebTransmitter(QObject* parent)
    : QObject(parent), d_ptr_(new MuebTransmitterPrivate(this)) {}

MuebTransmitter::~MuebTransmitter() { delete d_ptr_; }

void MuebTransmitter::SendFrame(libmueb::Frame frame) {
  Q_D(MuebTransmitter);

  if (frame.isNull() || frame.format() == QImage::Format_Invalid ||
      frame.width() != d->configuration_.width() ||
      frame.height() != d->configuration_.height()) {
    qWarning() << "[MuebTransmitter] Frame is invalid";
    return;
  }

  frame.convertTo(QImage::Format_RGB888);

  QByteArray reduced_compressed_frame;
  // Frame color reduction and compression
  if (d->configuration_.color_depth() < 5) {
    // FIXME Remove in Qt 6
    reduced_compressed_frame = QtConcurrent::blockingMappedReduced<QByteArray>(
        frame.constBits(), frame.constBits() + frame.sizeInBytes(),
        d->reduceColor, d->compressColor,
        QtConcurrent::OrderedReduce | QtConcurrent::SequentialReduce);

    /* FIXME Add in Qt 6
    reduced_compressed_frame = QtConcurrent::blockingMappedReduced<QByteArray>(
        frame.constBits(), frame.constBits() + frame.sizeInBytes(),
        // Reference:
        // http://threadlocalmutex.com/?p=48
        // http://threadlocalmutex.com/?page_id=60
        [d](const uchar& color) -> uchar {
          if (d->configuration_.color_depth() == 3) {
            return (color * 225 + 4096) >> 13;
          } else if (d->configuration_.color_depth() == 4) {
            return (color * 15 + 135) >> 8;
          }

          return color;
        },
        [d](QByteArray& compressed_colors, const uchar& color) {
          static bool msb{true};

          // Compress 2 color components into 1 byte
          if (msb) {
            compressed_colors.append(color << d->configuration_.factor());
          } else {
            compressed_colors.back() = compressed_colors.back() | color;
          }

          msb = !msb;
        },
        QtConcurrent::OrderedReduce | QtConcurrent::SequentialReduce);*/
  }
  // No compression
  else {
    reduced_compressed_frame.setRawData(
        reinterpret_cast<const char*>(frame.bits()), frame.sizeInBytes());
  }

  if (d->configuration_.max_packet_number() == 1) {
    reduced_compressed_frame.insert(0, d->configuration_.protocol_type())
        .insert(1, static_cast<char>(0) /*packet number*/);

    d->datagram_.setData(reduced_compressed_frame);
    d->socket_.writeDatagram(d->datagram_);
  } else {
    for (std::uint8_t i = 0; i < d->configuration_.max_packet_number(); ++i) {
      QByteArray data;
      data.append(d->configuration_.protocol_type())
          .append(i /*packet number*/)
          .append(reduced_compressed_frame.mid(
              i * d->configuration_.packet_payload_size(),
              d->configuration_.packet_payload_size()));
      /* FIXME in Qt6
      .append(reduced_compressed_frame.sliced(
          i * d->configuration_.packet_payload_size(),
          d->configuration_.packet_payload_size()));*/

      d->datagram_.setData(data);
      d->socket_.writeDatagram(d->datagram_);
    }
  }
}

MuebTransmitter& MuebTransmitter::Instance() {
  static MuebTransmitter instance;

  return instance;
}

std::int32_t MuebTransmitter::width() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.width();
}

std::int32_t MuebTransmitter::height() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.height();
}

libmueb::Frame MuebTransmitter::frame() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.frame();
}
