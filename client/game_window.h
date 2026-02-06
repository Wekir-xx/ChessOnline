#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "chess_board.h"

class GameWindow : public QWidget
{
public:
    explicit GameWindow(QWidget *parent = nullptr);

private:
    ChessBoard *m_board;
};

#endif // GAME_WINDOW_H
