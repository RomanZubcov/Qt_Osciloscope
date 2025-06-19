#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialReader : public QObject
{
    Q_OBJECT
public:
    explicit SerialReader(QObject *parent = nullptr);

    bool start(const QString &portName, qint32 baudRate = 115200);
    void stop();

signals:
    void newSample(double ch1, double ch2);
    void error(const QString &msg);

private slots:
    void onReadyRead();

private:
    QSerialPort m_serial;
    QByteArray m_buffer;
};

#endif // SERIALREADER_H
