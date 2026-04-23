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
    void writeSettingsParams();
    void writeStartParams();
    void writeChessBoardParams();
    void readSettingsParams();
    void readStartParams();
    void readChessBoardParams();

    void setBoard(QString board);
    QString getBoard();

    void setStyle();

private:
    StyleLib *m_styleLib;
    StartGameWindow *m_startGameWindow;
    GameWindow *m_gameWindow;
    BoardSetupWindow *m_boardSetupWindow;
    QStackedWidget *m_stacked;

    QScopedPointer<QSettings> m_settings;
    SettingsParams m_settingsParams;
    StartParams m_startParams;
    ChessBoardParams m_boardParams;
};

#endif // MAIN_WINDOW_H
