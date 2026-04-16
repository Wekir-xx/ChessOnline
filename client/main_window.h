#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "start_game_window.h"
#include "game_window.h"

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
    void readSettingsParams();
    void readStartParams();

    void setBoard(const QString board);
    const QString getBoard();

private:
    QStackedWidget *m_stacked;
    StartGameWindow *m_startGameWindow;
    GameWindow *m_gameWindow;

    QScopedPointer<QSettings> m_settings;

    GameParams m_params;
};

#endif // MAIN_WINDOW_H
