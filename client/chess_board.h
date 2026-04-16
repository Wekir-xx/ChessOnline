#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

//#define MOVE_PIECE

#include "chess_game.h"
#include "src/event_button.h"

#include <QLabel>
#include <QObject>
#include <QResizeEvent>
#include <QShowEvent>
#include <QGridLayout>
#include <QWidget>

#ifdef MOVE_PIECE
#include <QMouseEvent>
#endif

#include <unordered_map>

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);

    void resetBoard();

    void fillStandartChessBoard();
    void fill960ChessBoard();
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
    void clickField(const QString &nameField);

    void checkField(qint8 i, qint8 j);
    void moveField(qint8 i, qint8 j);
    void baseField(qint8 i, qint8 j);

    void checkLastMove();
    void uncheckLastMove();
    void checkKing();
    void uncheckKing();

    void takePiece(qint8 i, qint8 j);
    void untakePiece();

    void transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);
    void untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);

    void fillFullIcans();
    void fillIcan(bool white, bool up);
    void fillBoard();

    void updateChessScene();
    void updateHistoryScene();

private:
    ChessGame m_game;

    QGridLayout *m_board;
#ifdef MOVE_PIECE
    QLabel *m_movePiece;
#endif
    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<EventButton *>> m_chessBoardBut;
    std::vector<std::vector<QLabel *>> m_otherBoardLab;

    bool m_blockBoard{false};
    bool m_blockBoardHistory{false};
    bool m_transformPawn{false};
    bool m_autoQueen{false};
    bool m_premove{false};
    bool m_turnBoard{false};
    bool m_turnSecondPlayer{false};
    bool m_takenPiece{false};
    bool m_whiteMove{false};
};

#endif // CHESS_BOARD_H
