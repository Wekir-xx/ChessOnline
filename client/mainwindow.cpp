#include "mainwindow.h"

#define EMPTY 8
#define BEAT_FIELD_SIZE 0.3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setMinimumSize(QSize(405, 400));
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_imagesOfPieces.reserve(20);
    m_chessBoardLabels.resize(8, std::vector<QPushButton *>(8, nullptr));
    m_lastMove = {{EMPTY, 0}, {0, 0}};
    m_takenPiece = {EMPTY, 0};

    fillIcan();

    m_game.setChessParams(fillStandartChessBoard());

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

    for (qint8 i = 0; i < 8; ++i) {
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

    for (qint8 i = 0; i < 8; ++i) {
        QLabel *label = new QLabel(widget);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString(QChar('a' + i)));
        name_fields->addWidget(label);
    }

    for (qint8 i = 0; i < 8; ++i) {
        QVBoxLayout *layout = new QVBoxLayout();
        layout->setSpacing(0);

        for (qint8 j = 0; j < 8; ++j) {
            QPushButton *button = new QPushButton(widget);
            button->setObjectName(QString(QChar('a' + i)) + QString::number(8 - j));
            button->setSizePolicy(sizePolicy);

            layout->addWidget(button);
            m_chessBoardLabels[7 - j][i] = button;

            baseField(7 - j, i);

            connect(button, &QPushButton::clicked, this, [this, button]() {
                clickField(button->objectName());
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

    connect(&m_game, ChessGame::updateIconCastling, this, [&](short row, short col1, short col2) {
        if (row == 0)
            m_chessBoardLabels[row][col2]->setIcon(m_imagesOfPieces["wR"]);
        else
            m_chessBoardLabels[row][col2]->setIcon(m_imagesOfPieces["bR"]);

        m_chessBoardLabels[row][col1]->setIcon(QIcon());
    });

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

    for (qint8 i = 0; i < 8; ++i)
        for (qint8 j = 0; j < 8; ++j)
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());
}

void MainWindow::clickField(const QString &nameField)
{
    qint8 i = nameField[1].digitValue() - 1;
    qint8 j = nameField[0].unicode() - 'a';

    auto chessBoard = m_game.getChessBoard();

    if (m_takenPiece.first != EMPTY) {
        if (std::any_of(m_beatField.begin(), m_beatField.end(), [ = ](const auto & p) {
        return p.first == i && p.second == j;
    })) {
            if (m_lastMove.first.first != EMPTY) {
                baseField(m_lastMove.first.first, m_lastMove.first.second);
                baseField(m_lastMove.second.first, m_lastMove.second.second);
            }
            untakePiece();

            if (chessBoard[m_takenPiece.first][m_takenPiece.second][1] == 'P'
                && chessBoard[i][j].isEmpty() && m_takenPiece.second != j) {
                chessBoard[m_takenPiece.first][j].clear();
                m_chessBoardLabels[m_takenPiece.first][j]->setIcon(QIcon());
            }

            m_game.movePiece(std::pair{m_takenPiece.first, m_takenPiece.second}, std::pair{i, j});

            m_chessBoardLabels[i][j]->setIcon(
                m_imagesOfPieces[chessBoard[m_takenPiece.first][m_takenPiece.second]]);
            m_chessBoardLabels[m_takenPiece.first][m_takenPiece.second]->setIcon(QIcon());

            m_lastMove = {{i, j}, {m_takenPiece.first, m_takenPiece.second}};
            m_takenPiece.first = EMPTY;
            m_whiteMove ^= true;

            moveField(i, j);

            const auto &posKings = m_game.getPosKings();
            if (m_whiteMove) {
                baseField(posKings.second.first, posKings.second.second);

                if (m_game.isCheck()) {
                    checkField(posKings.first.first, posKings.first.second);

                    if (!m_game.isPossibleMove(m_lastMove))
                        this->setEnabled(false);
                } else if (!m_game.isPossibleMove(m_lastMove)) {
                    this->setEnabled(false);
                }
            } else {
                baseField(posKings.first.first, posKings.first.second);

                if (m_game.isCheck()) {
                    checkField(posKings.second.first, posKings.second.second);

                    if (!m_game.isPossibleMove(m_lastMove))
                        this->setEnabled(false);
                } else if (!m_game.isPossibleMove(m_lastMove)) {
                    this->setEnabled(false);
                }
            }
        } else if ((m_takenPiece.first != i || m_takenPiece.second != j)
                   && !chessBoard[i][j].isEmpty()
                   && ((m_whiteMove && chessBoard[i][j][0] == 'w')
                       || (!m_whiteMove && chessBoard[i][j][0] == 'b'))) {
            baseField(m_takenPiece.first, m_takenPiece.second);
            takePiece(i, j);
        } else {
            if (chessBoard[m_takenPiece.first][m_takenPiece.second][1] == 'K' && m_game.isCheck())
                checkField(m_takenPiece.first, m_takenPiece.second);
            else
                baseField(m_takenPiece.first, m_takenPiece.second);

            untakePiece();
            m_takenPiece.first = EMPTY;
        }
    } else {
        if (!chessBoard[i][j].isEmpty())
            if ((m_whiteMove && chessBoard[i][j][0] == 'w')
                || (!m_whiteMove && chessBoard[i][j][0] == 'b'))
                takePiece(i, j);
    }
}

void MainWindow::takePiece(qint8 i, qint8 j)
{
    untakePiece();
    m_takenPiece = {i, j};
    moveField(i, j);

    m_beatField = m_game.takePiece(i, j, m_lastMove);

    auto chessBoard = m_game.getChessBoard();

    for (const auto &field : m_beatField) {
        if (chessBoard[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(
                m_chessBoardLabels[field.first][field.second]->size() * BEAT_FIELD_SIZE);
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces["beatField"]);
        }
    }
}

void MainWindow::untakePiece()
{
    auto chessBoard = m_game.getChessBoard();

    for (const auto &field : m_beatField) {
        if (chessBoard[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(m_chessBoardLabels[field.first][field.second]->size());
            m_chessBoardLabels[field.first][field.second]->setIcon(QIcon());
        }
    }

    m_beatField.clear();
}

void MainWindow::checkField(qint8 i, qint8 j)
{
    m_chessBoardLabels[i][j]->setStyleSheet("background-color: #ff3838; border: none;");
}

void MainWindow::moveField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #B9CA43; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #F5F682; border: none;");
}

void MainWindow::baseField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #739552; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #EBECD0; border: none;");
}

void MainWindow::fillIcan()
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
    m_imagesOfPieces["beatField"] = QIcon(":/images/src/beatField.png");
}

void MainWindow::fillChessScene()
{
    auto chessBoard = m_game.getChessBoard();
    for (qint8 i = 0; i < 8; ++i) {
        for (qint8 j = 0; j < 8; ++j) {
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());
            if (chessBoard[i][j].isEmpty())
                continue;
            m_chessBoardLabels[i][j]->setIcon(m_imagesOfPieces[chessBoard[i][j]]);
        }
    }
}

ChessParams MainWindow::fillStandartChessBoard()
{
    ChessParams chess;
    chess.posKings = {{0, 4}, {7, 4}};
    chess.posRooksWhite = {{0, 0}, {0, 7}};
    chess.posRooksBlack = {{7, 0}, {7, 7}};

    std::vector<std::vector<QString>> chessBoard(8, std::vector<QString>(8));

    chessBoard[chess.posKings.first.first][chess.posKings.first.second] = "wK";
    chessBoard[0][3] = "wQ";
    chessBoard[chess.posRooksWhite.first.first][chess.posRooksWhite.first.second] = "wR";
    chessBoard[chess.posRooksWhite.second.first][chess.posRooksWhite.second.second] = "wR";
    chessBoard[0][2] = "wB";
    chessBoard[0][5] = "wB";
    chessBoard[0][1] = "wN";
    chessBoard[0][6] = "wN";

    chessBoard[chess.posKings.second.first][chess.posKings.second.second] = "bK";
    chessBoard[7][3] = "bQ";
    chessBoard[chess.posRooksBlack.first.first][chess.posRooksBlack.first.second] = "bR";
    chessBoard[chess.posRooksBlack.second.first][chess.posRooksBlack.second.second] = "bR";
    chessBoard[7][2] = "bB";
    chessBoard[7][5] = "bB";
    chessBoard[7][1] = "bN";
    chessBoard[7][6] = "bN";

    for (qint8 i = 0; i < 8; ++i) {
        chessBoard[1][i] = "wP";
        chessBoard[6][i] = "bP";
    }

    chess.chessBoard = chessBoard;

    return chess;
}
