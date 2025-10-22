#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
