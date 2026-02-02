#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chess_game.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>
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
    void takePiece(int i, int j);
    void untakePiece();
    void checkField(int i, int j);
    void uncheckField(int i, int j);
    void fillMap();
    void fillChessScene();
    std::vector<std::vector<QString>> fillStandartChessBoard();

private:
    // --- Fields ---

    ChessGame m_game;

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<QPushButton *>> m_chessBoardLabels;

    std::pair<int, int> m_takenPiece;
    std::vector<std::pair<int, int>> m_beatField;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_posKings;
    std::pair<std::pair<int, int>, std::pair<int, int>> m_lastMove;

    bool m_whiteMove{true};
};

#endif // MAINWINDOW_H
