#ifndef LIBMUEB_MUEBCHANNEL_H_
#define LIBMUEB_MUEBCHANNEL_H_

#include <QImage>
#include <QObject>

#include "muebreceiver.h"
#include "muebtransmitter.h"

namespace libmueb {

/*
    An instance of this class gets published over the WebChannel and is then
   accessible to HTML clients.
*/
class MuebChannel : public QObject {
  Q_OBJECT
  Q_PROPERTY(qint32 width READ width CONSTANT)
  Q_PROPERTY(qint32 height READ height CONSTANT)
  Q_PROPERTY(qint32 pixels READ pixels CONSTANT)

 public:
  explicit MuebChannel(QObject* parent = nullptr);

  qint32 width() const;

  qint32 height() const;

  quint32 pixels() const;

 signals:
  /*
        This signal is emitted from the C++ side and the text displayed on the
     HTML client side.
    */
  void SendFrame(QString frame);

 public slots:
  /*
        This slot is invoked from the HTML client side and the text displayed on
     the server side.
    */
  void ReceiveFrame(QString frame);

 private:
  MuebTransmitter& transmitter_;
  MuebReceiver& receiver_;

  void FrameChanged(QPixmap frame);
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBCHANNEL_H_
