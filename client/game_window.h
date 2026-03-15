#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "chess_board.h"
#include "src/board_layout.h"
#include "src/end_game_window.h"
#include "src/settings_window.h"

#include <QMessageBox>

class GameWindow : public QWidget
{
public:
    struct GameParams
    {
        SettingsWindow::SettingsParams settingParams;
        TypeGame gameType;
        TypeChess chessType;
        TypeTimeChess timeChessType;
        std::pair<qint16, qint8> mainTime;
        qint8 minorTime;
        std::vector<std::vector<QString>> chessFields;
        std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling;
    };

    explicit GameWindow(GameParams &params, QWidget *parent = nullptr);

    void showSettingWindow();
    void showEndGameWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void startGame(bool first = false);
    void endGame(ResultGame result);
    void newGame();
    void rematch();

private:
    ChessBoard *m_board;
    SettingsWindow *m_settings;
    EndGameWindow *m_endGame;
    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QMetaObject::Connection m_upButtonCon;
    QMetaObject::Connection m_downButtonCon;

    GameParams m_params;
    std::pair<QString, QString> m_nicknames;
};

#endif // GAME_WINDOW_H
