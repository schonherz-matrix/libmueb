#include "muebtransmitter.h"

#include <QList>

#include "muebtransmitter_p.h"

namespace libmueb {

struct CompressedColorsWrapper {
  QByteArray compressed_colors;
  bool msb{true};
};

MuebTransmitter::MuebTransmitter(QObject* parent)
    : QObject(parent), d_ptr_(new MuebTransmitterPrivate(this)) {}

MuebTransmitter::~MuebTransmitter() { delete d_ptr_; }

MuebTransmitter& MuebTransmitter::Instance() {
  static MuebTransmitter instance;

  return instance;
}

void MuebTransmitter::SendFrame(libmueb::Frame frame) {
  Q_D(MuebTransmitter);

  if (frame.isNull() || frame.format() == QImage::Format_Invalid ||
      frame.width() != d->configuration_.ConstFrame().width() ||
      frame.height() != d->configuration_.ConstFrame().height()) {
    qWarning() << "[MuebTransmitter] Frame is invalid";
    return;
  }

  frame.convertTo(QImage::Format_RGB888);
  QList<uchar> frame_bytes(frame.constBits(),
                           frame.constBits() + frame.sizeInBytes());

  // Frame color reduction and compression
  if (d->configuration_.color_depth() < 5) {
    auto result = QtConcurrent::mappedReduced<CompressedColorsWrapper>(
        frame_bytes,
        // Reference:
        // http://threadlocalmutex.com/?p=48
        // http://threadlocalmutex.com/?page_id=60
        [this](const uchar& color) -> uchar {
          Q_D(MuebTransmitter);
          uchar u_color = static_cast<uchar>(color);

          if (d->configuration_.color_depth() == 3) {
            return (u_color * 225 + 4096) >> 13;
          } else if (d->configuration_.color_depth() == 4) {
            return (u_color * 15 + 135) >> 8;
          }

          return u_color;
        },
        [](CompressedColorsWrapper& compressed_colors_wrapper,
           const uchar& color) {
          // Compress 2 color components into 1 byte
          if (compressed_colors_wrapper.msb) {
            compressed_colors_wrapper.compressed_colors.append(
                color << Configuration::kFactor);
          } else {
            compressed_colors_wrapper.compressed_colors.back() =
                compressed_colors_wrapper.compressed_colors.back() | color;
          }

          compressed_colors_wrapper.msb = !compressed_colors_wrapper.msb;
        },
        QtConcurrent::OrderedReduce | QtConcurrent::SequentialReduce);

    result.then(this,
                [this](CompressedColorsWrapper compressed_colors_wrapper) {
                  SendPacket(compressed_colors_wrapper.compressed_colors);
                });
  }
  // No compression
  else {
    SendPacket(QByteArray(reinterpret_cast<const char*>(frame.bits()),
                          frame.sizeInBytes()));
  }
}

quint32 MuebTransmitter::width() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.ConstFrame().width();
}

quint32 MuebTransmitter::height() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.ConstFrame().height();
}

quint32 MuebTransmitter::pixels() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.pixels();
}

quint32 MuebTransmitter::window_per_floor() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.window_per_floor();
}

quint32 MuebTransmitter::windows() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.windows();
}

quint32 MuebTransmitter::floors() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.floors();
}

quint8 MuebTransmitter::horizontal_pixel_unit() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.horizontal_pixel_unit();
}

quint8 MuebTransmitter::vertical_pixel_unit() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.vertical_pixel_unit();
}

qsizetype MuebTransmitter::FrameSizeInBytes() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.ConstFrame().sizeInBytes();
}

libmueb::Frame MuebTransmitter::frame() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.frame();
}

void MuebTransmitter::SendPacket(QByteArray reduced_compressed_frame) {
  Q_D(MuebTransmitter);

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
          .append(reduced_compressed_frame.sliced(
              i * d->configuration_.packet_payload_size(),
              d->configuration_.packet_payload_size()));

      d->datagram_.setData(data);
      d->socket_.writeDatagram(d->datagram_);
    }
  }
}
}  // namespace libmueb
