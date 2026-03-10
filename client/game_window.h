#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMessageBox>

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
        TypeTimeChess timeChessType;
        std::pair<qint16, qint8> mainTime;
        qint8 minorTime;
        std::vector<std::vector<QString>> chessFields;
        std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling;
    };

    explicit GameWindow(GameParams &params, QWidget *parent = nullptr);

    void showEndGameWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void startGame();
    void endGame(ResultGame result);
    void newGame();
    void rematch();

private:
    ChessBoard *m_board;
    EndGameWindow *m_endGame;
    QPushButton *m_upButton;
    QPushButton *m_downButton;

    GameParams m_params;
    std::pair<QString, QString> m_nicknames;
};

#endif // GAME_WINDOW_H
