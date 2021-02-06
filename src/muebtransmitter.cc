#include "muebtransmitter.h"

#include "muebtransmitter_p.h"

MuebTransmitter::MuebTransmitter(QObject* parent)
    : QObject(parent), d_ptr_(new MuebTransmitterPrivate(this)) {}

MuebTransmitter::~MuebTransmitter() { delete d_ptr_; }

void MuebTransmitter::SendFrame(libmueb::Frame frame) {
  Q_D(MuebTransmitter);

  if (frame.isNull() || frame.format() == QImage::Format_Invalid ||
      frame.width() != d->configuration_.width() ||
      frame.height() != d->configuration_.height()) {
    qWarning() << "[MuebTransmitter] Frame is invalid";
    return;
  }

  frame.convertTo(QImage::Format_RGB888);
  d->SendFrame(frame);
}

MuebTransmitter& MuebTransmitter::Instance() {
  static MuebTransmitter instance;

  return instance;
}

std::int32_t MuebTransmitter::width() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.width();
}

std::int32_t MuebTransmitter::height() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.height();
}

libmueb::Frame MuebTransmitter::frame() const {
  Q_D(const MuebTransmitter);

  return d->configuration_.frame();
}
