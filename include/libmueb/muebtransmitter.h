#ifndef LIBMUEB_MUEBTRANSMITTER_H_
#define LIBMUEB_MUEBTRANSMITTER_H_

#include <QByteArray>
#include <QObject>

#include "libmueb_global.h"

namespace libmueb {

class MuebTransmitterPrivate;

class LIBMUEB_EXPORT MuebTransmitter final : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE_D(d_ptr_, MuebTransmitter)
  Q_DISABLE_COPY(MuebTransmitter)

 public:
  static MuebTransmitter& Instance();

  void SendFrame(libmueb::Frame frame);

  quint32 width() const;

  quint32 height() const;

  quint32 pixels() const;

  quint32 window_per_floor() const;

  quint32 windows() const;

  quint32 floors() const;

  quint8 horizontal_pixel_unit() const;

  quint8 vertical_pixel_unit() const;

  qsizetype FrameSizeInBytes() const;

  libmueb::Frame frame() const;

 private:
  MuebTransmitterPrivate* d_ptr_;

  explicit MuebTransmitter(QObject* parent = nullptr);
  ~MuebTransmitter();

  void SendPacket(QByteArray reduced_compressed_frame);
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBTRANSMITTER_H_
