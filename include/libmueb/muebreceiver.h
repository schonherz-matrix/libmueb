#ifndef LIBMUEB_MUEBRECEIVER_H_
#define LIBMUEB_MUEBRECEIVER_H_

#include <QObject>
#include <QPixmap>

#include "libmueb_global.h"

class MuebReceiverPrivate;

class LIBMUEB_EXPORT MuebReceiver final : public QObject {
  Q_OBJECT
  Q_DECLARE_PRIVATE_D(d_ptr_, MuebReceiver)
  Q_DISABLE_COPY(MuebReceiver)

 public:
  static MuebReceiver& Instance();
  QPixmap frame() const;

 signals:
  void FrameChanged(QPixmap frame);

 private:
  MuebReceiverPrivate* d_ptr_;

  explicit MuebReceiver(QObject* parent = nullptr);
  ~MuebReceiver();

  void ReadPendingDatagrams();
};

#endif  // LIBMUEB_MUEBRECEIVER_H_
