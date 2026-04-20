#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

//#define MOVE_PIECE

#include "chess_game.h"
#include "src/chess_board_widget.h"

#include <QLabel>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>

#ifdef MOVE_PIECE
#include <QMouseEvent>
#endif

#include <unordered_map>

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(StyleLib *styleLib, QWidget *parent = nullptr);

    void updateBoard();

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

    void updateBoardIcon();
    void updateBoardSize();
    void updateHistoryScene();

private:
    ChessGame m_game;

    ChessBoardWidget *m_chessBoard;

    QVBoxLayout *m_mainLayout;

#ifdef MOVE_PIECE
    QLabel *m_movePiece;
#endif

    bool m_blockBoard{false};
    bool m_blockBoardHistory{false};
    bool m_transformPawn{false};
    bool m_autoQueen{false};
    bool m_premove{false};
    bool m_takenPiece{false};
    bool m_whiteMove{false};
};

#endif // CHESS_BOARD_H
