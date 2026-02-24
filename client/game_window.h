#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "chess_board.h"
#include "src/board_layout.h"
#include "src/end_game_window.h"

class GameWindow : public QWidget
{
public:
    struct GameParams
    {
        TypeGame gameType;
        TypeChess chessType;
        std::pair<qint16, qint8> mainTime;
        qint8 minorTime;
    };

    explicit GameWindow(GameParams &params, QWidget *parent = nullptr);

    void showEndGameWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    ChessBoard *m_board;
    EndGameWindow *m_endGame;

    GameParams _params;
    std::pair<QString, QString> _nicknames;
};

#endif // GAME_WINDOW_H
