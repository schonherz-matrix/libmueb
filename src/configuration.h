#ifndef LIBMUEB_CONFIGURATION_H_
#define LIBMUEB_CONFIGURATION_H_

#include <QHostAddress>
#include <QImage>
#include <QNetworkInterface>
#include <QObject>
#include <QSettings>
#include <QtGlobal>

namespace libmueb {

class Configuration final {
  Q_DISABLE_COPY(Configuration)

 public:
  static constexpr quint8 kFactor{4};

  static Configuration& Instance();

  QImage frame() const;

  const QImage& ConstFrame() const;

  QHostAddress target_address() const;

  quint32 pixels() const;

  quint8 protocol_type() const;

  quint32 packet_header_size() const;

  quint32 packet_size() const;

  quint32 packet_payload_size() const;

  quint32 frame_fragment_size() const;

  quint16 animation_port() const;

  quint8 max_packet_number() const;

  quint8 color_depth() const;

  QNetworkInterface multicast_interface() const;

 private:
  QImage frame_;
  QNetworkInterface multicast_interface_;
  QHostAddress target_address_;
  quint32 pixels_;
  quint32 packet_header_size_;
  quint32 packet_size_;
  quint32 packet_payload_size_;
  quint32 frame_fragment_size_;
  quint16 animation_port_;
  quint8 max_packet_number_;
  quint8 color_depth_;

  Configuration();
  ~Configuration() = default;
};

}  // namespace libmueb

#endif  // LIBMUEB_CONFIGURATION_H_
