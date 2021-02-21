#include "configuration.h"

#include <QtMath>

// TODO check, remove unused parameters
// TODO check, variable types
Configuration::Configuration(QObject *parent)
    : QObject(parent),
      settings_(QSettings::IniFormat, QSettings::UserScope, "matrix-group",
                "libmueb") {
  LoadSettings();
}

QImage Configuration::frame() const { return frame_; }

QHostAddress Configuration::target_address() const { return target_address_; }

quint32 Configuration::floors() const { return floors_; }

quint32 Configuration::rooms_per_floor() const { return rooms_per_floor_; }

quint32 Configuration::windows_per_room() const { return windows_per_room_; }

quint32 Configuration::vertical_pixel_unit() const {
  return vertical_pixel_unit_;
}

quint32 Configuration::horizontal_pixel_unit() const {
  return horizontal_pixel_unit_;
}

quint32 Configuration::pixels_per_window() const { return pixels_per_window_; }

quint32 Configuration::window_per_floor() const { return window_per_floor_; }

quint32 Configuration::windows() const { return windows_; }

quint32 Configuration::pixels() const { return pixels_; }

qint32 Configuration::width() const { return width_; }

qint32 Configuration::height() const { return height_; }

quint8 Configuration::protocol_type() const { return protocol_type_; }

quint32 Configuration::window_byte_size() const { return window_byte_size_; }

quint32 Configuration::max_windows_per_datagram() const {
  return max_windows_per_datagram_;
}

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

quint32 Configuration::remainder_packet_size() const {
  return remainder_packet_size_;
}

quint16 Configuration::broadcast_animation_port() const {
  return broadcast_animation_port_;
}

quint8 Configuration::max_packet_number() const { return max_packet_number_; }

quint8 Configuration::color_depth() const { return color_depth_; }

quint8 Configuration::factor() const { return factor_; }

bool Configuration::debug_mode() const { return debug_mode_; }

void Configuration::LoadSettings() {
  // Building specific constants
  settings_.beginGroup("building");
  floors_ = settings_.value("floors", 13).toUInt();
  rooms_per_floor_ = settings_.value("rooms_per_floor", 8).toUInt();
  windows_per_room_ = settings_.value("windows_per_room", 2).toUInt();
  settings_.endGroup();

  // Hardware specific constants
  settings_.beginGroup("hardware");
  vertical_pixel_unit_ = settings_.value("vertical_pixel_unit", 2).toUInt();
  horizontal_pixel_unit_ = settings_.value("horizontal_pixel_unit", 2).toUInt();
  color_depth_ = settings_.value("color_depth", 3).toUInt();
  settings_.endGroup();

  pixels_per_window_ = vertical_pixel_unit_ * horizontal_pixel_unit_;
  window_per_floor_ = rooms_per_floor_ * windows_per_room_;
  windows_ = floors_ * window_per_floor_;
  pixels_ = windows_ * pixels_per_window_;
  width_ = window_per_floor_ * horizontal_pixel_unit_;
  height_ = floors_ * vertical_pixel_unit_;
  factor_ = 8 - color_depth_;

  frame_ = QImage(width_, height_, QImage::Format_RGB888);
  frame_.fill(Qt::black);

  // Network protocol specific constants
  settings_.beginGroup("network");
  protocol_type_ = 2;
  broadcast_animation_port_ =
      settings_.value("broadcast_animation_port", 10000).toUInt();

  // Send packets to localhost
  debug_mode_ = settings_.value("debug_mode", false).toBool();

  target_address_ =
      (debug_mode_)
          ? QHostAddress("127.0.0.1")
          : QHostAddress(
                settings_.value("target_address", "10.6.255.255").toString());
  window_byte_size_ = (color_depth_ >= 3 && color_depth_ < 5)
                          ? pixels_per_window_ * 3 / 2
                          : pixels_per_window_ * 3;
  max_windows_per_datagram_ =
      settings_.value("max_windows_per_datagram", windows_).toUInt();
  packet_header_size_ = 2;
  packet_size_ =
      packet_header_size_ + max_windows_per_datagram_ * window_byte_size_;
  packet_payload_size_ = max_windows_per_datagram_ * window_byte_size_;
  frame_fragment_size_ = max_windows_per_datagram_ * pixels_per_window_ * 3;
  max_packet_number_ =
      qCeil(static_cast<qreal>(windows_) / max_windows_per_datagram_);
  settings_.endGroup();

  // TODO Configuration check
}
