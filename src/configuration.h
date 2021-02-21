#ifndef LIBMUEB_CONFIGURATION_H_
#define LIBMUEB_CONFIGURATION_H_

#include <QHostAddress>
#include <QImage>
#include <QObject>
#include <QSettings>
#include <QtGlobal>

class Configuration : public QObject {
  Q_OBJECT

 public:
  explicit Configuration(QObject *parent = nullptr);

  QImage frame() const;

  QHostAddress target_address() const;

  quint32 pixels() const;

  qint32 width() const;

  qint32 height() const;

  quint8 protocol_type() const;

  quint32 packet_header_size() const;

  quint32 packet_size() const;

  quint32 packet_payload_size() const;

  quint32 frame_fragment_size() const;

  quint16 broadcast_animation_port() const;

  quint8 max_packet_number() const;

  quint8 color_depth() const;

  quint8 factor() const;

  bool debug_mode() const;

 private:
  QImage frame_;
  QSettings settings_;
  QHostAddress target_address_;
  quint32 floors_;
  quint32 rooms_per_floor_;
  quint32 windows_per_room_;
  quint32 vertical_pixel_unit_;
  quint32 horizontal_pixel_unit_;
  quint32 pixels_per_window_;
  quint32 window_per_floor_;
  quint32 windows_;
  quint32 pixels_;
  // Qt width, height is signed
  qint32 width_;
  qint32 height_;
  //
  quint8 protocol_type_;
  quint32 window_byte_size_;
  quint32 max_windows_per_datagram_;
  quint32 packet_header_size_;
  quint32 packet_size_;
  quint32 packet_payload_size_;
  quint32 frame_fragment_size_;
  quint16 broadcast_animation_port_;
  quint8 max_packet_number_;
  quint8 color_depth_;
  quint8 factor_;
  bool debug_mode_;

  void LoadSettings();
};

#endif  // LIBMUEB_CONFIGURATION_H_
