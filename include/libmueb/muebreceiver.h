#ifndef LIBMUEB_MUEBRECEIVER_H_
#define LIBMUEB_MUEBRECEIVER_H_

#include <QImage>
#include <QObject>

#include "libmueb_global.h"

namespace libmueb {

class MuebReceiverPrivate;

class LIBMUEB_EXPORT MuebReceiver final : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE_D(d_ptr_, MuebReceiver)
  Q_DISABLE_COPY(MuebReceiver)
  Q_PROPERTY(QImage frame READ frame NOTIFY FrameChanged)

 public:
  static MuebReceiver& Instance();

  QImage frame() const;

  quint8 horizontal_pixel_unit() const;

  quint8 vertical_pixel_unit() const;

 signals:
  void FrameChanged(QImage frame);

 private:
  MuebReceiverPrivate* d_ptr_;

  explicit MuebReceiver(QObject* parent = nullptr);
  ~MuebReceiver();

  void ReadPendingDatagrams();
};

}  // namespace libmueb

#endif  // LIBMUEB_MUEBRECEIVER_H_
