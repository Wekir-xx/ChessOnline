#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    this->resize(700, 700);
    this->setWindowTitle("Chess Server");

    m_mainWidget = new MainWidget();
    this->setCentralWidget(m_mainWidget);
}
