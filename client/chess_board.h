#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include "chess_game.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QWidget>

#include <unordered_map>

class ChessBoard : public QWidget
{
public:
    explicit ChessBoard(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void clickField(const QString &nameField);
    void checkField(qint8 i, qint8 j);
    void moveField(qint8 i, qint8 j);
    void baseField(qint8 i, qint8 j);

    void takePiece(qint8 i, qint8 j);
    void untakePiece();

    void transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);
    void untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);

    void fillIcan();
    void updateChessScene();
    ChessParams fillStandartChessBoard();

private:
    ChessGame m_game;

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<QPushButton *>> m_chessBoardLabels;

    bool m_transformPawn{false};
};

#endif // CHESS_BOARD_H
