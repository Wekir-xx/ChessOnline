#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "game_window.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    GameWindow *m_gameWindow;
};

#endif // MAIN_WINDOW_H
