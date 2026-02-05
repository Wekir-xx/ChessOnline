#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include <QChar>
#include <QDebug>
#include <QString>

#include <vector>

#define EMPTY -1

struct ChessParams
{
    std::vector<std::vector<QString>> chessBoard;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posKings;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksWhite;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksBlack;
    bool chess960;
};

class ChessGame
{
public:
    ChessGame();

    void movePiece(std::pair<qint8, qint8> oldPos, std::pair<qint8, qint8> newPos);
    std::vector<std::pair<qint8, qint8>>& takePiece(qint8 i, qint8 j);
    bool isCheck();
    bool isPossibleMove();

    void setChessParams(ChessParams chess);

    std::vector<std::vector<QString>>& getChessBoard();
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>>& getLastMove();
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>>& getPosKings();

private:
    bool checkMove(qint8 i, qint8 j, bool isKing = false);
    void addCastling();
    void addMovesRook();
    void addMovesBishop();

private:
    ChessParams m_chess;
    std::pair<qint8, qint8> m_takenPiece;
    std::vector<std::pair<qint8, qint8>> m_beatField;
    std::pair<std::pair<bool, bool>, std::pair<bool, bool>> m_castling;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> m_lastMove;

    bool m_whiteMove{true};
};

#endif // CHESS_GAME_H
