#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "start_game_window.h"
#include "game_window.h"

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *m_stacked;
    StartGameWindow *m_startGameWindow;
    GameWindow *m_gameWindow;

    GameParams m_params;
};

#endif // MAIN_WINDOW_H
