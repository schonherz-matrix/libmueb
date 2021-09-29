#include <QApplication>
#include <QLabel>
#include <QTimer>

#include "muebreceiver.h"

class Task : public QObject {
  Q_OBJECT

 public:
  Task(QObject *parent = 0)
      : QObject(parent), receiver_(libmueb::MuebReceiver::Instance()) {
    connect(&receiver_, &libmueb::MuebReceiver::FrameChanged, this,
            &Task::TestFrame);
    label_.setFixedSize(320, 260);
    label_.show();
  }

 private:
  libmueb::MuebReceiver &receiver_;
  QLabel label_;

  void TestFrame(QImage frame) {
    label_.setPixmap(QPixmap::fromImage(frame.scaled(320, 260)));
  }
};

#include "testmuebreceiver.moc"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Task task;

  return a.exec();
}
