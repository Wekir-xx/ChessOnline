#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "../defines.h"

#include <vector>

class ChessGame
{
private:

    struct baseInfo {
        PieceInfo info;
        std::pair<int, int> posKing;
        std::pair<std::pair<int, int>, std::pair<int, int>> posRooks;

        baseInfo (bool white)
            : info(white),
              posKing{SIDE_SIZE, SIDE_SIZE},
              posRooks{{SIDE_SIZE, SIDE_SIZE}, {SIDE_SIZE, SIDE_SIZE}}
        {}
    };

public:
    ChessGame();

    void movePiece(std::pair<int, int> newPos);
    void takePiece(int i, int j);
    void untakePiece();

    bool isPossibleMove();
    bool isStaleMate();
    bool isCheck();

    bool isPossibleHistoryBack();
    bool isPossibleHistoryForward();
    void historyMove();

    void chooseTransformPawn(int j);
    void transformPawn(int i, int j);
    void untransformPawn();

    void setField(QString field, int i, int j);
    void setColorMove(bool whiteMove);
    void setCheck(bool flag);
    void setChessParams(ChessBoardParams &boardParams);

    bool getCheck();
    bool getColorMove();
    std::pair<int, int> getTakenPiece();
    const std::vector<std::vector<QString>> &getChessFields();
    const std::vector<std::pair<int, int>> &getBeatFields();
    std::pair<std::pair<int, int>, std::pair<int, int>> getLastMove();
    std::pair<std::pair<int, int>, std::pair<int, int>> getPosKings();

private:
    QString castlingKing(std::pair<int, int> newPos, baseInfo& info, std::pair<bool, bool>& castling);
    void moveRook(const baseInfo& info, std::pair<bool, bool>& castling);

    bool checkMove(int i, int j, bool isKing = false);
    bool checkEnPassant(int toRow, int toCol, int capRow, int capCol);
    bool isPossibleMoveInner();

    void addCastling();
    void addMoves(const std::pair<int, int> *moves);

private:

    static constexpr std::pair<int, int> RookMoves[4] = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    static constexpr std::pair<int, int> BishopMoves[4] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    static constexpr std::pair<int, int> KnightMoves[8] = {{2,1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {1,-2}, {-1,2}, {-1,-2}};

    ChessBoardParams m_boardParams;

    baseInfo m_whiteInfo;
    baseInfo m_blackInfo;

    bool m_whiteMove{true};
    bool m_check{false};

    std::vector<QString> m_savePiece;
    std::pair<int, int> m_takenPiece;
    std::vector<std::pair<int, int>> m_beatFields;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_lastMove;

    std::vector<std::vector<std::vector<QString>>> m_chessFieldsHistory;
    std::vector<QString> m_chessMoveHistory;
    size_t m_numBoardHistory;
    size_t m_movesHistory;
};

#endif // CHESS_GAME_H
