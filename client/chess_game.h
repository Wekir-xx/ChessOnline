#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "src/defines.h"

#include <QChar>

#include <vector>

class ChessGame
{
public:
    struct ChessParams {
        std::vector<std::vector<QString>> chessFields;
        std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posKings;
        std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksWhite;
        std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> posRooksBlack;
        bool chess960;
    };

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
    void setChessParams(ChessParams chess, std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling);

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
    ChessParams m_chess;
    std::vector<QString> m_savePiece;
    std::pair<qint8, qint8> m_takenPiece;
    std::vector<std::pair<qint8, qint8>> m_beatFields;
    std::vector<QString> m_chessMoveHistory;
    std::vector<std::vector<std::vector<QString>>> m_chessFieldsHistory;
    std::pair<std::pair<bool, bool>, std::pair<bool, bool>> m_castling;
    std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> m_lastMove;

    bool m_whiteMove{true};
    bool m_check{false};
    qint8 m_movesHistory;
    qint16 m_numBoardHistory;
};

#endif // CHESS_GAME_H
