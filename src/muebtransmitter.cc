#include "muebtransmitter.h"

#include "muebtransmitter_p.h"

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
