#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <QDebug>
#include <QString>

#include <vector>

class ChessGame
{
public:
    ChessGame();

    void movePiece(std::pair<int, int> oldPos, std::pair<int, int> newPos);
    std::vector<std::pair<int, int>>& takePiece(
        int i, int j, std::pair<std::pair<int, int>, std::pair<int, int>> m_lastMove);
    bool isCheck();
    bool isMate();

    void setChessBoard(std::vector<std::vector<QString>> chessBoard);
    void setPosKings(std::pair<std::pair<int, int>, std::pair<int, int>> posKings);

    std::vector<std::vector<QString>>& getChessBoard();

private:
    bool checkMove(int i, int j, bool isKing = false);
    void addMovesRook();
    void addMovesBishop();

private:
    std::vector<std::vector<QString>> m_chessBoard;
    std::vector<std::pair<int, int>> m_beatField;
    std::pair<int, int> m_takenPiece;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_posKings;

    bool m_whiteMove{true};
};

#endif // CHESS_GAME_H
