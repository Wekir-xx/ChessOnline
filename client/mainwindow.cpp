#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_imagesOfPieces.reserve(20);
    m_chessBoard.resize(8, std::vector<QString>(8));

    fillMap();
    fillStandartChessBoard();
    fillChessScene();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->a1->setPixmap(m_imagesOfPieces[m_chessBoard[0][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a2->setPixmap(m_imagesOfPieces[m_chessBoard[1][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a3->setPixmap(m_imagesOfPieces[m_chessBoard[2][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a4->setPixmap(m_imagesOfPieces[m_chessBoard[3][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a5->setPixmap(m_imagesOfPieces[m_chessBoard[4][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a6->setPixmap(m_imagesOfPieces[m_chessBoard[5][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a7->setPixmap(m_imagesOfPieces[m_chessBoard[6][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->a8->setPixmap(m_imagesOfPieces[m_chessBoard[7][0]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->b1->setPixmap(m_imagesOfPieces[m_chessBoard[0][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b2->setPixmap(m_imagesOfPieces[m_chessBoard[1][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b3->setPixmap(m_imagesOfPieces[m_chessBoard[2][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b4->setPixmap(m_imagesOfPieces[m_chessBoard[3][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b5->setPixmap(m_imagesOfPieces[m_chessBoard[4][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b6->setPixmap(m_imagesOfPieces[m_chessBoard[5][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b7->setPixmap(m_imagesOfPieces[m_chessBoard[6][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->b8->setPixmap(m_imagesOfPieces[m_chessBoard[7][1]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->c1->setPixmap(m_imagesOfPieces[m_chessBoard[0][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c2->setPixmap(m_imagesOfPieces[m_chessBoard[1][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c3->setPixmap(m_imagesOfPieces[m_chessBoard[2][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c4->setPixmap(m_imagesOfPieces[m_chessBoard[3][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c5->setPixmap(m_imagesOfPieces[m_chessBoard[4][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c6->setPixmap(m_imagesOfPieces[m_chessBoard[5][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c7->setPixmap(m_imagesOfPieces[m_chessBoard[6][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->c8->setPixmap(m_imagesOfPieces[m_chessBoard[7][2]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->d1->setPixmap(m_imagesOfPieces[m_chessBoard[0][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d2->setPixmap(m_imagesOfPieces[m_chessBoard[1][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d3->setPixmap(m_imagesOfPieces[m_chessBoard[2][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d4->setPixmap(m_imagesOfPieces[m_chessBoard[3][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d5->setPixmap(m_imagesOfPieces[m_chessBoard[4][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d6->setPixmap(m_imagesOfPieces[m_chessBoard[5][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d7->setPixmap(m_imagesOfPieces[m_chessBoard[6][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->d8->setPixmap(m_imagesOfPieces[m_chessBoard[7][3]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->e1->setPixmap(m_imagesOfPieces[m_chessBoard[0][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e2->setPixmap(m_imagesOfPieces[m_chessBoard[1][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e3->setPixmap(m_imagesOfPieces[m_chessBoard[2][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e4->setPixmap(m_imagesOfPieces[m_chessBoard[3][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e5->setPixmap(m_imagesOfPieces[m_chessBoard[4][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e6->setPixmap(m_imagesOfPieces[m_chessBoard[5][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e7->setPixmap(m_imagesOfPieces[m_chessBoard[6][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->e8->setPixmap(m_imagesOfPieces[m_chessBoard[7][4]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->f1->setPixmap(m_imagesOfPieces[m_chessBoard[0][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f2->setPixmap(m_imagesOfPieces[m_chessBoard[1][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f3->setPixmap(m_imagesOfPieces[m_chessBoard[2][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f4->setPixmap(m_imagesOfPieces[m_chessBoard[3][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f5->setPixmap(m_imagesOfPieces[m_chessBoard[4][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f6->setPixmap(m_imagesOfPieces[m_chessBoard[5][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f7->setPixmap(m_imagesOfPieces[m_chessBoard[6][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->f8->setPixmap(m_imagesOfPieces[m_chessBoard[7][5]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->g1->setPixmap(m_imagesOfPieces[m_chessBoard[0][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g2->setPixmap(m_imagesOfPieces[m_chessBoard[1][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g3->setPixmap(m_imagesOfPieces[m_chessBoard[2][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g4->setPixmap(m_imagesOfPieces[m_chessBoard[3][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g5->setPixmap(m_imagesOfPieces[m_chessBoard[4][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g6->setPixmap(m_imagesOfPieces[m_chessBoard[5][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g7->setPixmap(m_imagesOfPieces[m_chessBoard[6][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->g8->setPixmap(m_imagesOfPieces[m_chessBoard[7][6]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->h1->setPixmap(m_imagesOfPieces[m_chessBoard[0][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h2->setPixmap(m_imagesOfPieces[m_chessBoard[1][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h3->setPixmap(m_imagesOfPieces[m_chessBoard[2][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h4->setPixmap(m_imagesOfPieces[m_chessBoard[3][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h5->setPixmap(m_imagesOfPieces[m_chessBoard[4][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h6->setPixmap(m_imagesOfPieces[m_chessBoard[5][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h7->setPixmap(m_imagesOfPieces[m_chessBoard[6][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->h8->setPixmap(m_imagesOfPieces[m_chessBoard[7][7]].scaled(ui->a1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
