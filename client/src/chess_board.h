#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

//#define MOVE_PIECE

#include "chess_game.h"
#include "chess_board_widget.h"

#include <QLabel>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>

#ifdef MOVE_PIECE
#include <QMouseEvent>
#endif

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(StyleLib *styleLib, QWidget *parent = nullptr);

    void updateIconBoard();

    void fillChessBoard(bool chess960);
    void fillUserChessBoard(ChessBoardParams &boardParams);

    void turnBoard();
    void turnSecondPlayer();
    void historyBack();
    void historyForward();

    void setBlockBoard(bool flag);
    void setAutoQueen(bool flag);
    void setPremove(bool flag);

    bool getBlockBoard();
    bool getColorMove();
    bool getTurnBoard();
    bool getTurnSecondPlayer();

signals:
    void didMove();
    void endGame(ResultGame result);

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

#ifdef MOVE_PIECE
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
#endif

private:
    void clickField(QString nameField);

    void uncheckLastMove();
    void checkKing();
    void uncheckKing();

    void takePiece(qint8 i, qint8 j);
    void untakePiece();

    void transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);
    void untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);

    void updateIcon();
    void updateBoard();
    void updateBoardSize();
    void updateHistoryScene();

private:
    StyleLib *m_styleLib;
    ChessGame m_game;
    ChessBoardWidget *m_chessBoard;

    QVBoxLayout *m_mainLayout;

#ifdef MOVE_PIECE
    QLabel *m_movePiece;
    bool m_takenPiece;
#endif

    bool m_blockBoard;
    bool m_blockBoardHistory;
    bool m_transformPawn;
    bool m_autoQueen;
    bool m_premove;
    bool m_whiteMove;
};

#endif // CHESS_BOARD_H
