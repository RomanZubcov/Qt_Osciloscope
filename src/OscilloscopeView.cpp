#include "OscilloscopeView.h"
#include <QPainter>
#include <QPaintEvent>

OscilloscopeView::OscilloscopeView(QWidget *parent)
    : QWidget(parent), m_maxSamples(1024), m_zoomX(1.0), m_zoomY(1.0), m_triggerLevel(0), m_triggered(false)
{
    m_ch1.resize(m_maxSamples);
    m_ch2.resize(m_maxSamples);
    connect(&m_refreshTimer, &QTimer::timeout, this, &OscilloscopeView::onRefresh);
    m_refreshTimer.start(30); // roughly 33 FPS
}

void OscilloscopeView::addSample(double ch1, double ch2)
{
    QMutexLocker locker(&m_mutex);
    m_ch1.push_back(ch1);
    m_ch2.push_back(ch2);
    if (m_ch1.size() > m_maxSamples) {
        m_ch1.remove(0, m_ch1.size() - m_maxSamples);
        m_ch2.remove(0, m_ch2.size() - m_maxSamples);
    }
}

void OscilloscopeView::setZoomX(double factor) { m_zoomX = factor; }
void OscilloscopeView::setZoomY(double factor) { m_zoomY = factor; }
void OscilloscopeView::setTriggerLevel(double level) { m_triggerLevel = level; }

void OscilloscopeView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);

    p.fillRect(rect(), Qt::black);

    // Draw grid
    p.setPen(QPen(QColor(40, 40, 40)));
    const int gridSpacing = 50;
    for (int x = 0; x < width(); x += gridSpacing)
        p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += gridSpacing)
        p.drawLine(0, y, width(), y);

    p.setClipRect(rect());

    // Draw channels
    QMutexLocker locker(&m_mutex);
    if (m_ch1.isEmpty())
        return;

    const int sampleCount = m_ch1.size();
    auto mapX = [&](int idx) {
        return width() - (sampleCount - idx) * m_zoomX;
    };
    auto mapY = [&](double val) {
        return height()/2 - val * m_zoomY;
    };

    p.setRenderHint(QPainter::Antialiasing);
    // Channel 1
    p.setPen(QPen(Qt::green));
    QPainterPath path1;
    path1.moveTo(mapX(0), mapY(m_ch1[0]));
    for (int i = 1; i < sampleCount; ++i)
        path1.lineTo(mapX(i), mapY(m_ch1[i]));
    p.drawPath(path1);

    // Channel 2
    p.setPen(QPen(Qt::red));
    QPainterPath path2;
    path2.moveTo(mapX(0), mapY(m_ch2[0]));
    for (int i = 1; i < sampleCount; ++i)
        path2.lineTo(mapX(i), mapY(m_ch2[i]));
    p.drawPath(path2);

    // Trigger line
    p.setPen(QPen(Qt::yellow, 1, Qt::DashLine));
    int trigY = mapY(m_triggerLevel);
    p.drawLine(0, trigY, width(), trigY);
}

void OscilloscopeView::onRefresh()
{
    update();
}

