#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OscilloscopeView.h"
#include "SerialReader.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onSample(double ch1, double ch2);

private:
    OscilloscopeView *m_view;
    SerialReader m_reader;
};

#endif // MAINWINDOW_H
