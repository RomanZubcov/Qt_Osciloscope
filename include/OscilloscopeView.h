#ifndef OSCILLOSCOPEVIEW_H
#define OSCILLOSCOPEVIEW_H

#include <QWidget>
#include <QVector>
#include <QMutex>
#include <QTimer>

class OscilloscopeView : public QWidget
{
    Q_OBJECT
public:
    explicit OscilloscopeView(QWidget *parent = nullptr);

    // Adds a sample to the circular buffer
    void addSample(double ch1, double ch2);

    void setZoomX(double factor); // time axis
    void setZoomY(double factor); // voltage axis

    void setTriggerLevel(double level); // trigger level for channel1

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onRefresh();

private:
    QVector<double> m_ch1;
    QVector<double> m_ch2;
    int m_maxSamples;

    double m_zoomX;
    double m_zoomY;
    double m_triggerLevel;
    bool m_triggered;

    QMutex m_mutex;
    QTimer m_refreshTimer;
};

#endif // OSCILLOSCOPEVIEW_H
