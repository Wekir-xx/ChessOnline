#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    this->resize(500, 500);
    this->setWindowTitle("Chess Server");
    this->setWindowIcon(QIcon(":/src/settings.png"));

    m_mainWidget = new MainWidget();
    this->setCentralWidget(m_mainWidget);
}
