#ifndef LIBMUEB_CONFIGURATION_H_
#define LIBMUEB_CONFIGURATION_H_

#include <QHostAddress>
#include <QImage>
#include <QObject>
#include <QSettings>
#include <cstdint>

// TODO check, remove unused parameters
// TODO check, variable types
class Configuration : public QObject {
  Q_OBJECT

 public:
  explicit Configuration(QObject *parent = nullptr);

  QImage frame() const;

  QHostAddress target_address() const;

  std::uint32_t floors() const;

  std::uint32_t rooms_per_floor() const;

  std::uint32_t windows_per_room() const;

  std::uint32_t vertical_pixel_unit() const;

  std::uint32_t horizontal_pixel_unit() const;

  std::uint32_t pixels_per_window() const;

  std::uint32_t window_per_floor() const;

  std::uint32_t windows() const;

  std::uint32_t pixels() const;

  std::int32_t width() const;

  std::int32_t height() const;

  std::uint8_t protocol_type() const;

  std::uint32_t window_byte_size() const;

  std::uint32_t max_windows_per_datagram() const;

  std::uint32_t packet_header_size() const;

  std::uint32_t packet_size() const;

  std::uint32_t packet_payload_size() const;

  std::uint32_t frame_fragment_size() const;

  std::uint32_t remainder_packet_size() const;

  std::uint16_t unicast_animation_port() const;

  std::uint16_t broadcast_animation_port() const;

  std::uint8_t max_packet_number() const;

  std::uint8_t color_depth() const;

  std::uint8_t factor() const;

  bool debug_mode() const;

 private:
  QImage frame_;
  QSettings settings_;
  QHostAddress target_address_;
  std::uint32_t floors_;
  std::uint32_t rooms_per_floor_;
  std::uint32_t windows_per_room_;
  std::uint32_t vertical_pixel_unit_;
  std::uint32_t horizontal_pixel_unit_;
  std::uint32_t pixels_per_window_;
  std::uint32_t window_per_floor_;
  std::uint32_t windows_;
  std::uint32_t pixels_;
  // Qt width, height is signed
  std::int32_t width_;
  std::int32_t height_;
  //
  std::uint8_t protocol_type_;
  std::uint32_t window_byte_size_;
  std::uint32_t max_windows_per_datagram_;
  std::uint32_t packet_header_size_;
  std::uint32_t packet_size_;
  std::uint32_t packet_payload_size_;
  std::uint32_t frame_fragment_size_;
  std::uint32_t remainder_packet_size_;
  std::uint16_t unicast_animation_port_;
  std::uint16_t broadcast_animation_port_;
  std::uint8_t max_packet_number_;
  std::uint8_t color_depth_;
  std::uint8_t factor_;
  bool debug_mode_;

  void LoadSettings();
};

#endif  // LIBMUEB_CONFIGURATION_H_
