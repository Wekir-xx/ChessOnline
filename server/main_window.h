#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "main_widget.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    MainWidget *m_mainWidget;
};

#endif // MAINWINDOW_H
