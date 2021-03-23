#include <QCoreApplication>
#include <QObject>
#include <QThread>
#include <QTimer>

#include "muebtransmitter.h"

class Task : public QObject {
  Q_OBJECT

 public:
  Task(QObject *parent = 0) : QObject(parent) {}

 public slots:
  void run() {
    libmueb::MuebTransmitter &transmitter =
        libmueb::MuebTransmitter::Instance();
    libmueb::Frame frame = transmitter.frame();

    // Test RGB
    frame.fill(Qt::red);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    frame.fill(Qt::green);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    frame.fill(Qt::blue);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    frame.fill(Qt::cyan);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    frame.fill(Qt::yellow);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    frame.fill(Qt::magenta);
    transmitter.SendFrame(frame);
    QThread::sleep(1);

    // Row test
    for (int row = 0; row < 26; ++row) {
      frame.fill(Qt::black);
      for (int col = 0; col < 32; ++col) {
        frame.setPixelColor(col, row, Qt::red);
      }
      transmitter.SendFrame(frame);
      QThread::msleep(250);
    }

    // Column test
    for (int col = 0; col < 32; ++col) {
      frame.fill(Qt::black);
      for (int row = 0; row < 26; ++row) {
        frame.setPixelColor(col, row, Qt::red);
      }
      transmitter.SendFrame(frame);
      QThread::msleep(250);
    }

    emit finished();
  }

 signals:
  void finished();
};

#include "testmuebtransmitter.moc"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Task task;
  QObject::connect(&task, &Task::finished, &a, &QCoreApplication::quit);
  QTimer::singleShot(0, &task, &Task::run);

  return a.exec();
}
