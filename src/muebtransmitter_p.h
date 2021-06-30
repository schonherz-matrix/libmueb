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

  // FIXME Remove in Qt 6
  /* Reference:
   * http://threadlocalmutex.com/?p=48
   * http://threadlocalmutex.com/?page_id=60
   */
  std::function<uchar(const uchar&)> reduceColor =
      [this](const uchar& color) -> uchar {
    if (configuration_.color_depth() == 3) {
      return (color * 225 + 4096) >> 13;
    } else if (configuration_.color_depth() == 4) {
      return (color * 15 + 135) >> 8;
    }

    return color;
  };

  // FIXME Remove in Qt 6
  std::function<void(QByteArray&, const uchar&)> compressColor =
      [this](QByteArray& compressed_colors, const uchar& color) {
        static bool msb{true};

        // Compress 2 color components into 1 byte
        if (msb) {
          compressed_colors.append(color << Configuration::kFactor);
        } else {
          compressed_colors.back() = compressed_colors.back() | color;
        }

        msb = !msb;
      };
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBTRANSMITTER_P_H_
