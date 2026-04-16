#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "board_setup_window.h"
#include "game_window.h"
#include "start_game_window.h"

#include <QMainWindow>
#include <QSettings>
#include <QScopedPointer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void switchStartGameWindow();

    void writeSettingsParams();
    void writeStartParams();
    void writeChessBoardParams();
    void readSettingsParams();
    void readStartParams();
    void readChessBoardParams();

    void setBoard(QString board);
    QString getBoard();

private:
    QStackedWidget *m_stacked;
    StartGameWindow *m_startGameWindow;
    GameWindow *m_gameWindow;
    BoardSetupWindow *m_boardSetupWindow;

    QScopedPointer<QSettings> m_settings;

    GameParams m_params;
};

#endif // MAIN_WINDOW_H
