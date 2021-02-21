#include "muebchannel.h"

#include <QBuffer>
#include <QByteArray>
#include <QDebug>
#include <QString>

MuebChannel::MuebChannel(QObject *parent)
    : QObject(parent),
      transmitter_(MuebTransmitter::Instance()),
      receiver_(MuebReceiver::Instance()) {
  connect(&receiver_, &MuebReceiver::FrameChanged, this,
          &MuebChannel::FrameChanged);
}

qint32 MuebChannel::width() const { return transmitter_.width(); }

qint32 MuebChannel::height() const { return transmitter_.height(); }

void MuebChannel::ReceiveFrame(QString frame) {
  auto idx = frame.indexOf(',');

  if (idx > 0) {
    frame = frame.mid(idx);
  }

  if (auto result = QByteArray::fromBase64Encoding(frame.toLatin1())) {
    transmitter_.SendFrame(QImage::fromData(*result, "PNG"));
  }
}

void MuebChannel::FrameChanged(QPixmap frame) {
  QByteArray ba;
  QBuffer buf(&ba);
  buf.open(QIODevice::WriteOnly);
  frame.save(&buf, "PNG");
  QByteArray base64 = ba.toBase64().prepend("data:image/png;base64,");
  buf.close();

  emit SendFrame(base64);
}
