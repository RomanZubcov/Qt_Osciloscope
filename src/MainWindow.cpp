#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_view(new OscilloscopeView)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(m_view);
    setCentralWidget(central);

    connect(&m_reader, &SerialReader::newSample, this, &MainWindow::onSample);
    // Example: start reading from COM1 with default baud
    // m_reader.start("COM1");
}

void MainWindow::onSample(double ch1, double ch2)
{
    m_view->addSample(ch1, ch2);
}

