#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_chessBoardLabels = {
        {ui->a1, ui->b1, ui->c1, ui->d1, ui->e1, ui->f1, ui->g1, ui->h1},
        {ui->a2, ui->b2, ui->c2, ui->d2, ui->e2, ui->f2, ui->g2, ui->h2},
        {ui->a3, ui->b3, ui->c3, ui->d3, ui->e3, ui->f3, ui->g3, ui->h3},
        {ui->a4, ui->b4, ui->c4, ui->d4, ui->e4, ui->f4, ui->g4, ui->h4},
        {ui->a5, ui->b5, ui->c5, ui->d5, ui->e5, ui->f5, ui->g5, ui->h5},
        {ui->a6, ui->b6, ui->c6, ui->d6, ui->e6, ui->f6, ui->g6, ui->h6},
        {ui->a7, ui->b7, ui->c7, ui->d7, ui->e7, ui->f7, ui->g7, ui->h7},
        {ui->a8, ui->b8, ui->c8, ui->d8, ui->e8, ui->f8, ui->g8, ui->h8}
    };


    m_imagesOfPieces.reserve(20);

    fillMap();
    fillStandartChessBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    fillChessScene();
}

void MainWindow::fillMap()
{
    m_imagesOfPieces["wK"] = QPixmap(":/images/src/wKing.png");
    m_imagesOfPieces["wQ"] = QPixmap(":/images/src/wQueen.png");
    m_imagesOfPieces["wR"] = QPixmap(":/images/src/wRook.png");
    m_imagesOfPieces["wB"] = QPixmap(":/images/src/wBishop.png");
    m_imagesOfPieces["wN"] = QPixmap(":/images/src/wKnight.png");
    m_imagesOfPieces["wP"] = QPixmap(":/images/src/wPawn.png");
    m_imagesOfPieces["bK"] = QPixmap(":/images/src/bKing.png");
    m_imagesOfPieces["bQ"] = QPixmap(":/images/src/bQueen.png");
    m_imagesOfPieces["bR"] = QPixmap(":/images/src/bRook.png");
    m_imagesOfPieces["bB"] = QPixmap(":/images/src/bBishop.png");
    m_imagesOfPieces["bN"] = QPixmap(":/images/src/bKnight.png");
    m_imagesOfPieces["bP"] = QPixmap(":/images/src/bPawn.png");
}

void MainWindow::fillStandartChessBoard()
{
    m_chessBoard.resize(8, std::vector<QString>(8));

    m_chessBoard[0][4] = "wK";
    m_chessBoard[0][3] = "wQ";
    m_chessBoard[0][0] = "wR";
    m_chessBoard[0][7] = "wR";
    m_chessBoard[0][1] = "wB";
    m_chessBoard[0][6] = "wB";
    m_chessBoard[0][2] = "wN";
    m_chessBoard[0][5] = "wN";

    m_chessBoard[7][3] = "bQ";
    m_chessBoard[7][4] = "bK";
    m_chessBoard[7][0] = "bR";
    m_chessBoard[7][7] = "bR";
    m_chessBoard[7][1] = "bB";
    m_chessBoard[7][6] = "bB";
    m_chessBoard[7][2] = "bN";
    m_chessBoard[7][5] = "bN";

    for (size_t i = 0; i < 8; ++i) {
        m_chessBoard[1][i] = "wP";
        m_chessBoard[6][i] = "bP";
    }
}

void MainWindow::fillChessScene()
{
    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            if(m_chessBoard[i][j] == "")
                continue;
            m_chessBoardLabels[i][j]->setPixmap(m_imagesOfPieces[m_chessBoard[i][j]]
                                                        .scaled(m_chessBoardLabels[i][j]->size(),
                                                                Qt::KeepAspectRatio,
                                                                Qt::SmoothTransformation));
        }
    }
}
