#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "chess_game.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QSizePolicy>
#include <QTransform>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QWidget>

#include <unordered_map>

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);

    void resetBoard();

    void fillStandartChessBoard();
    void fillStandart960ChessBoard();
    void fillUserChessBoard(std::vector<std::vector<QString>> chessFields,
                            bool chess960,
                            std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling);

    void turnBoard();
    void turnChess();
    void historyBack();
    void historyForward();

    void setBlockBoard(bool flag);
    void setAutoQueen(bool flag);
    void setPremove(bool flag);

    bool getBlockBoard();
    bool getColorMove();

signals:
    void endGame(ResultGame result);

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

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
    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<QPushButton *>> m_chessBoardBut;
    std::vector<std::vector<QLabel *>> m_otherBoardLab;

    bool m_blockBoard{false};
    bool m_blockBoardHistory{false};
    bool m_transformPawn{false};
    bool m_autoQueen{false};
    bool m_premove{false};
    bool m_turnBoard{false};
    bool m_turnChess{false};
};

#endif // CHESS_BOARD_H
