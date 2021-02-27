#include "configuration.h"

#include <QtMath>

namespace libmueb {

namespace {
constexpr quint8 kRgbByteSize{3};
constexpr quint8 kProtocolType{2};
}  // namespace

Configuration::Configuration(QObject *parent) : QObject(parent) {
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "matrix-group",
                     "libmueb");
  // Building specific constants
  settings.beginGroup("building");
  quint32 floors = settings.value("floors", 13).toUInt();
  quint32 rooms_per_floor = settings.value("rooms_per_floor", 8).toUInt();
  quint32 windows_per_room = settings.value("windows_per_room", 2).toUInt();
  settings.endGroup();

  // Hardware specific constants
  settings.beginGroup("hardware");
  quint32 vertical_pixel_unit =
      settings.value("vertical_pixel_unit", 2).toUInt();
  quint32 horizontal_pixel_unit =
      settings.value("horizontal_pixel_unit", 2).toUInt();
  color_depth_ = settings.value("color_depth", 3).toUInt();
  settings.endGroup();

  quint32 pixels_per_window = vertical_pixel_unit * horizontal_pixel_unit;
  quint32 window_per_floor = rooms_per_floor * windows_per_room;
  quint32 windows = floors * window_per_floor;
  pixels_ = windows * pixels_per_window;
  factor_ = 8 - color_depth_;
  // Alpha channel is not supported by hardware
  // The image is stored using a 24-bit RGB format (8-8-8)
  frame_ = QImage(window_per_floor * horizontal_pixel_unit,
                  floors * vertical_pixel_unit, QImage::Format_RGB888);
  frame_.fill(Qt::black);

  // Network protocol specific constants
  settings.beginGroup("network");
  broadcast_animation_port_ =
      settings.value("broadcast_animation_port", 10000).toUInt();

  // Send packets to localhost
  debug_mode_ = settings.value("debug_mode", false).toBool();

  target_address_ =
      (debug_mode_)
          ? QHostAddress("127.0.0.1")
          : QHostAddress(
                settings.value("target_address", "10.6.255.255").toString());
  quint32 window_byte_size = (color_depth_ >= 3 && color_depth_ < 5)
                                 ? pixels_per_window * kRgbByteSize / 2
                                 : pixels_per_window * kRgbByteSize;
  quint32 max_windows_per_datagram =
      settings.value("max_windows_per_datagram", windows).toUInt();
  packet_header_size_ = 2;
  packet_size_ =
      packet_header_size_ + max_windows_per_datagram * window_byte_size;
  packet_payload_size_ = max_windows_per_datagram * window_byte_size;
  // Uncompressed frame fragment byte size
  frame_fragment_size_ =
      max_windows_per_datagram * pixels_per_window * kRgbByteSize;
  max_packet_number_ =
      qCeil(static_cast<qreal>(windows) / max_windows_per_datagram);
  settings.endGroup();

  // TODO Configuration check
}

Configuration &Configuration::Instance() {
  static Configuration instance;

  return instance;
}

QImage Configuration::frame() const { return frame_; }

const QImage &Configuration::ConstFrame() const { return frame_; }

QHostAddress Configuration::target_address() const { return target_address_; }

quint32 Configuration::pixels() const { return pixels_; }

quint8 Configuration::protocol_type() const { return kProtocolType; }

quint32 Configuration::packet_header_size() const {
  return packet_header_size_;
}

quint32 Configuration::packet_size() const { return packet_size_; }

quint32 Configuration::packet_payload_size() const {
  return packet_payload_size_;
}

quint32 Configuration::frame_fragment_size() const {
  return frame_fragment_size_;
}

quint16 Configuration::broadcast_animation_port() const {
  return broadcast_animation_port_;
}

quint8 Configuration::max_packet_number() const { return max_packet_number_; }

quint8 Configuration::color_depth() const { return color_depth_; }

quint8 Configuration::factor() const { return factor_; }

bool Configuration::debug_mode() const { return debug_mode_; }

}  // namespace libmueb
