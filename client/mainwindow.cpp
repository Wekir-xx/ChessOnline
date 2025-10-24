#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setMinimumSize(QSize(405, 400));
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_imagesOfPieces.reserve(20);
    m_chessBoardLabels.resize(8, std::vector<QPushButton *>(8, nullptr));
    m_lastBeat = std::pair{std::pair{8, 8}, std::pair{8, 8}};

    fillMap();
    fillStandartChessBoard();

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);

    QWidget *widget = new QWidget(this);

    QHBoxLayout *chess_board_layout = new QHBoxLayout(widget);
    chess_board_layout->setSpacing(0);

    QVBoxLayout *chess_field_name_layout = new QVBoxLayout();
    chess_field_name_layout->setSpacing(0);

    QHBoxLayout *chess_field_layout = new QHBoxLayout();
    chess_field_layout->setSpacing(0);

    QVBoxLayout *row_layout = new QVBoxLayout();
    row_layout->setSpacing(0);

    QHBoxLayout *name_fields = new QHBoxLayout();
    name_fields->setSpacing(0);

    for (int i = 0; i < 8; ++i) {
        QLabel *label = new QLabel(widget);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString::number(8 - i));
        row_layout->addWidget(label);
        row_layout->setStretch(i, 10);
    }

    QLabel *empty = new QLabel(widget);
    empty->setSizePolicy(sizePolicy);
    row_layout->addWidget(empty);
    row_layout->setStretch(8, 5);

    for (int i = 0; i < 8; ++i) {
        QLabel *label = new QLabel(widget);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString(QChar('a' + i)));
        name_fields->addWidget(label);
    }

    for (int i = 0; i < 8; ++i) {
        QVBoxLayout *layout = new QVBoxLayout();
        layout->setSpacing(0);

        for (int j = 0; j < 8; ++j) {
            QPushButton *button = new QPushButton(widget);
            button->setObjectName(QString(QChar('a' + i)) + QString::number(8 - j));
            button->setSizePolicy(sizePolicy);

            layout->addWidget(button);
            m_chessBoardLabels[7 - j][i] = button;

            uncheckField(7 - j, i);

            connect(button, &QPushButton::clicked, this, [this, button]() {
                clickPiece(button->objectName());
            });
        }

        chess_field_layout->addLayout(layout);
    }

    chess_field_name_layout->addLayout(chess_field_layout);
    chess_field_name_layout->addLayout(name_fields);

    chess_field_name_layout->setStretch(0, 16);
    chess_field_name_layout->setStretch(1, 1);

    chess_board_layout->addLayout(row_layout);
    chess_board_layout->addLayout(chess_field_name_layout);

    chess_board_layout->setStretch(0, 1);
    chess_board_layout->setStretch(1, 19);

    this->setCentralWidget(widget);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    fillChessScene();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());
}

void MainWindow::clickPiece(const QString &nameField)
{
    int i = nameField[1].digitValue() - 1;
    int j = nameField[0].unicode() - 'a';

    const auto &cord = m_takenPiece.first;

    if (m_takenPiece.second.isEmpty()) {
        if (!m_chessBoard[i][j].isEmpty()) {
            if ((m_whiteMove && m_chessBoard[i][j][0] == 'w')
                    || (!m_whiteMove && m_chessBoard[i][j][0] == 'b')) {
                m_takenPiece = std::pair{std::pair{i, j}, m_chessBoard[i][j]};
                checkField(i, j);
            }
        }
    } else if ((m_whiteMove && m_chessBoard[i][j][0] == 'b')
               || (!m_whiteMove && m_chessBoard[i][j][0] == 'w') || m_chessBoard[i][j].isEmpty()) {
        if (m_lastBeat.first.first != 8) {
            uncheckField(m_lastBeat.first.first, m_lastBeat.first.second);
            uncheckField(m_lastBeat.second.first, m_lastBeat.second.second);
        }

        m_chessBoard[i][j] = m_takenPiece.second;
        m_chessBoardLabels[i][j]->setIcon(m_imagesOfPieces[m_chessBoard[i][j]]);
        checkField(i, j);
        m_whiteMove ^= true;

        m_lastBeat = std::pair{std::pair{i, j}, std::pair{cord.first, cord.second}};

        m_chessBoard[cord.first][cord.second].clear();
        m_chessBoardLabels[cord.first][cord.second]->setIcon(QIcon());
        m_takenPiece.second.clear();
    } else if (cord.first == i && cord.second == j) {
        uncheckField(cord.first, cord.second);
        m_takenPiece.second.clear();
    } else {
        uncheckField(cord.first, cord.second);
        m_takenPiece = std::pair{std::pair{i, j}, m_chessBoard[i][j]};
        checkField(i, j);
    }
}

void MainWindow::checkField(int i, int j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #B9CA43; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #F5F682; border: none;");
}

void MainWindow::uncheckField(int i, int j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #739552; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #EBECD0; border: none;");
}

void MainWindow::fillMap()
{
    m_imagesOfPieces["wK"] = QIcon(":/images/src/wKing.png");
    m_imagesOfPieces["wQ"] = QIcon(":/images/src/wQueen.png");
    m_imagesOfPieces["wR"] = QIcon(":/images/src/wRook.png");
    m_imagesOfPieces["wB"] = QIcon(":/images/src/wBishop.png");
    m_imagesOfPieces["wN"] = QIcon(":/images/src/wKnight.png");
    m_imagesOfPieces["wP"] = QIcon(":/images/src/wPawn.png");
    m_imagesOfPieces["bK"] = QIcon(":/images/src/bKing.png");
    m_imagesOfPieces["bQ"] = QIcon(":/images/src/bQueen.png");
    m_imagesOfPieces["bR"] = QIcon(":/images/src/bRook.png");
    m_imagesOfPieces["bB"] = QIcon(":/images/src/bBishop.png");
    m_imagesOfPieces["bN"] = QIcon(":/images/src/bKnight.png");
    m_imagesOfPieces["bP"] = QIcon(":/images/src/bPawn.png");
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

    for (int i = 0; i < 8; ++i) {
        m_chessBoard[1][i] = "wP";
        m_chessBoard[6][i] = "bP";
    }
}

void MainWindow::fillChessScene()
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());
            if (m_chessBoard[i][j].isEmpty())
                continue;
            m_chessBoardLabels[i][j]->setIcon(m_imagesOfPieces[m_chessBoard[i][j]]);
        }
    }
}
