#ifndef LIBMUEB_MUEBTRANSMITTER_H_
#define LIBMUEB_MUEBTRANSMITTER_H_

#include <QObject>

#include "libmueb_global.h"

class MuebTransmitterPrivate;

class LIBMUEB_EXPORT MuebTransmitter final : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE_D(d_ptr_, MuebTransmitter)
  Q_DISABLE_COPY(MuebTransmitter)

 public:
  void SendFrame(libmueb::Frame frame);

  static MuebTransmitter& Instance();

  quint32 width() const;

  quint32 height() const;

  quint32 pixels() const;

  qsizetype FrameSizeInBytes() const;

  libmueb::Frame frame() const;

 private:
  MuebTransmitterPrivate* d_ptr_;

  explicit MuebTransmitter(QObject* parent = nullptr);
  ~MuebTransmitter();
};

#endif  // LIBMUEB_MUEBTRANSMITTER_H_
