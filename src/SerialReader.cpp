#include "SerialReader.h"
#include <QTextStream>

SerialReader::SerialReader(QObject *parent)
    : QObject(parent)
{
    connect(&m_serial, &QSerialPort::readyRead, this, &SerialReader::onReadyRead);
}

bool SerialReader::start(const QString &portName, qint32 baudRate)
{
    if (m_serial.isOpen())
        m_serial.close();

    m_serial.setPortName(portName);
    m_serial.setBaudRate(baudRate);

    if (!m_serial.open(QIODevice::ReadOnly)) {
        emit error(tr("Cannot open port %1").arg(portName));
        return false;
    }

    m_buffer.clear();
    return true;
}

void SerialReader::stop()
{
    if (m_serial.isOpen())
        m_serial.close();
}

void SerialReader::onReadyRead()
{
    m_buffer.append(m_serial.readAll());
    int idx;
    while ((idx = m_buffer.indexOf('\n')) != -1) {
        QByteArray line = m_buffer.left(idx).trimmed();
        m_buffer.remove(0, idx + 1);
        QList<QByteArray> parts = line.split(',');
        if (parts.size() >= 2) {
            bool ok1, ok2;
            double ch1 = parts[0].toDouble(&ok1);
            double ch2 = parts[1].toDouble(&ok2);
            if (ok1 && ok2)
                emit newSample(ch1, ch2);
        }
    }
}

