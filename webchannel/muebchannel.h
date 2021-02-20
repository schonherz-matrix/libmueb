#ifndef LIBMUEB_MUEBTRANSMITTERCHANNEL_H_
#define LIBMUEB_MUEBTRANSMITTERCHANNEL_H_

#include <QImage>
#include <QObject>

#include "muebreceiver.h"
#include "muebtransmitter.h"

/*
    An instance of this class gets published over the WebChannel and is then
   accessible to HTML clients.
*/
class MuebChannel : public QObject {
  Q_OBJECT

 public:
  explicit MuebChannel(QObject* parent = nullptr);

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

#endif  // LIBMUEB_MUEBTRANSMITTERCHANNEL_H_
