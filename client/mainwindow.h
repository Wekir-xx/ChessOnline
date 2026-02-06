#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chess_game.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QVBoxLayout>
#include <QWidget>

#include <unordered_map>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // --- Constructors/destructors ---

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    // --- Helper methods ---

    void clickField(const QString &nameField);
    void takePiece(qint8 i, qint8 j);
    void untakePiece();
    void checkField(qint8 i, qint8 j);
    void moveField(qint8 i, qint8 j);
    void baseField(qint8 i, qint8 j);
    void transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);
    void untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields);
    void fillIcan();
    void updateChessScene();
    ChessParams fillStandartChessBoard();

private:
    // --- Fields ---

    ChessGame m_game;

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<QPushButton *>> m_chessBoardLabels;

    bool m_transformPawn{false};
};

#endif // MAINWINDOW_H
