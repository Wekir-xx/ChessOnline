#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "src/defines.h"

#include <vector>

class ChessGame
{
public:
    ChessGame();

    void movePiece(std::pair<qint8, qint8> newPos);
    void takePiece(qint8 i, qint8 j);
    void untakePiece();

    bool isPossibleMove();
    bool isStaleMate();

    bool isPossibleHistoryBack();
    bool isPossibleHistoryForward();
    void historyMove();

    void chooseTransformPawn(qint8 j);
    void transformPawn(qint8 i, qint8 j);
    void untransformPawn();

    void setField(QString field, qint8 i, qint8 j);
    void setChessParams(ChessBoardParams &boardParams, ChessPosParams &posParams);

    bool getCheck();
    bool getColorMove();
    std::pair<qint8, qint8> getTakenPiece();
    const std::vector<std::vector<QString>> &getChessFields();
    const std::vector<std::pair<qint8, qint8>> &getBeatFields();
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> getLastMove();
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> getPosKings();

private:
    bool isCheck();
    bool checkMove(qint8 i, qint8 j, bool isKing = false);
    bool isPossibleMoveInner();

    void addCastling();
    void addMovesRook();
    void addMovesBishop();

private:
    ChessBoardParams m_boardParams;
    ChessPosParams m_posParams;

    std::vector<QString> m_savePiece;
    std::pair<qint8, qint8> m_takenPiece;
    std::vector<std::pair<qint8, qint8>> m_beatFields;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> m_lastMove;

    std::vector<std::vector<std::vector<QString>>> m_chessFieldsHistory;
    std::vector<QString> m_chessMoveHistory;
    size_t m_numBoardHistory;
    qint8 m_movesHistory;

    bool m_whiteMove{true};
    bool m_check{false};
};

#endif // CHESS_GAME_H
