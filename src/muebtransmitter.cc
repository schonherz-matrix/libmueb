#include "muebtransmitter.h"

#include <QByteArray>
#include <QDebug>
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QtConcurrent>

#include "configuration.h"

class MuebTransmitterPrivate {
  Q_DECLARE_PUBLIC(MuebTransmitter)
  Q_DISABLE_COPY(MuebTransmitterPrivate)

 public:
  explicit MuebTransmitterPrivate(MuebTransmitter* q)
      : datagram_(QByteArray(), configuration_.target_address(),
                  configuration_.broadcast_animation_port()),
        q_ptr(q) {
    qInfo().noquote() << "[MuebTransmitter] UDP Socket will send frame to"
                      << QString("%1:%2")
                             .arg(configuration_.target_address().toString())
                             .arg(configuration_.broadcast_animation_port());
  }

  void SendFrame(libmueb::Frame frame) {
    std::uint8_t packet_number{0};

    QByteArray reduced_compressed_frame;
    // Frame color reduction and compression
    if (configuration_.color_depth() < 5) {
      reduced_compressed_frame =
          QtConcurrent::blockingMappedReduced<QByteArray>(
              frame.constBits(), frame.constBits() + frame.sizeInBytes(),
              /* Reference:
               * http://threadlocalmutex.com/?p=48
               * http://threadlocalmutex.com/?page_id=60
               */
              [this](const uchar& color) -> uchar {
                if (configuration_.color_depth() == 3) {
                  return (color * 225 + 4096) >> 13;
                } else if (configuration_.color_depth() == 4) {
                  return (color * 15 + 135) >> 8;
                }

                return color;
              },
              [this](QByteArray& compressed_colors, const uchar& color) {
                static bool msb{true};

                // Compress 2 color components into 1 byte
                if (msb) {
                  compressed_colors.append(color << configuration_.factor());
                } else {
                  compressed_colors.back() = compressed_colors.back() | color;
                }

                msb = !msb;
              },
              QtConcurrent::OrderedReduce | QtConcurrent::SequentialReduce);
    }
    // No compression
    else {
      reduced_compressed_frame.setRawData(
          reinterpret_cast<const char*>(frame.bits()), frame.sizeInBytes());
    }

    if (configuration_.max_packet_number() == 1) {
      reduced_compressed_frame.insert(0, configuration_.protocol_type())
          .insert(1, packet_number);

      datagram_.setData(reduced_compressed_frame);
    } else {
      for (std::uint8_t i = 0; i < configuration_.max_packet_number(); ++i) {
        QByteArray data;
        data.append(configuration_.protocol_type())
            .append(packet_number++)
            .append(reduced_compressed_frame.sliced(
                i * configuration_.frame_fragment_size(),
                configuration_.frame_fragment_size()));

        datagram_.setData(data);
      }
    }

    socket_.writeDatagram(datagram_);
  }

  Configuration configuration_;
  QUdpSocket socket_;
  QNetworkDatagram datagram_;
  MuebTransmitter* q_ptr;
};

MuebTransmitter::MuebTransmitter(QObject* parent)
    : QObject(parent), d_ptr_(new MuebTransmitterPrivate(this)) {}

MuebTransmitter::~MuebTransmitter() { delete d_ptr_; }

void MuebTransmitter::SendFrame(libmueb::Frame frame) {
  if (frame.isNull() || frame.format() == QImage::Format_Invalid ||
      frame.width() != d_ptr_->configuration_.width() ||
      frame.height() != d_ptr_->configuration_.height()) {
    qWarning() << "[MuebTransmitter] Frame is invalid";
    return;
  }

  frame.convertTo(QImage::Format_RGB888);
  d_ptr_->SendFrame(frame);
}

MuebTransmitter& MuebTransmitter::Instance() {
  static MuebTransmitter instance;

  return instance;
}

std::int32_t MuebTransmitter::width() const {
  return d_ptr_->configuration_.width();
}

std::int32_t MuebTransmitter::height() const {
  return d_ptr_->configuration_.height();
}

libmueb::Frame MuebTransmitter::frame() const {
  return d_ptr_->configuration_.frame();
}
