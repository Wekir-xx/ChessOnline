#include "mainwindow.h"

#define BEAT_FIELD_SIZE 0.35
#define BEAT_FIELD_OPACITY 0.27

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setMinimumSize(QSize(405, 400));
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_imagesOfPieces.reserve(20);
    m_chessBoardLabels.resize(8, std::vector<QPushButton *>(8, nullptr));
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

            connect(button, &QPushButton::clicked, this, [this, button]() { clickField(button->objectName()); });
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
    updateChessScene();
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

    QChar color = 'w';
    if (!m_whiteMove)
        color = 'b';

    auto chessBoard = m_game.getChessBoard();

    if (m_takenPiece.first != EMPTY) {
        if (std::any_of(m_beatField.begin(), m_beatField.end(), [=](const auto &p) { return p.first == i && p.second == j; })) {
            const auto &lastMove = m_game.getLastMove();
            if (lastMove.first.first != EMPTY) {
                baseField(lastMove.first.first, lastMove.first.second);
                baseField(lastMove.second.first, lastMove.second.second);
            }

            untakePiece();

            const auto &posKings = m_game.getPosKings();
            if (m_game.isCheck() && chessBoard[m_takenPiece.first][m_takenPiece.second][1] != 'K') {
                if (m_whiteMove)
                    baseField(posKings.first.first, posKings.first.second);
                else
                    baseField(posKings.second.first, posKings.second.second);
            }

            m_game.movePiece(std::pair{m_takenPiece.first, m_takenPiece.second}, std::pair{i, j});
            moveField(lastMove.second.first, lastMove.second.second);
            updateChessScene();

            m_takenPiece.first = EMPTY;
            m_whiteMove ^= true;

            if (m_whiteMove) {
                if (m_game.isCheck()) {
                    checkField(posKings.first.first, posKings.first.second);

                    if (!m_game.isPossibleMove())
                        this->setEnabled(false);
                } else if (!m_game.isPossibleMove()) {
                    this->setEnabled(false);
                }
            } else {
                if (m_game.isCheck()) {
                    checkField(posKings.second.first, posKings.second.second);

                    if (!m_game.isPossibleMove())
                        this->setEnabled(false);
                } else if (!m_game.isPossibleMove()) {
                    this->setEnabled(false);
                }
            }
        } else {
            if (chessBoard[m_takenPiece.first][m_takenPiece.second][1] == 'K' && m_game.isCheck())
                checkField(m_takenPiece.first, m_takenPiece.second);
            else
                baseField(m_takenPiece.first, m_takenPiece.second);

            if ((m_takenPiece.first != i || m_takenPiece.second != j) && !chessBoard[i][j].isEmpty() && chessBoard[i][j][0] == color) {
                takePiece(i, j);
            } else {
                untakePiece();
                m_takenPiece.first = EMPTY;
            }
        }
    } else if (!chessBoard[i][j].isEmpty() && chessBoard[i][j][0] == color) {
        takePiece(i, j);
    }
}

void MainWindow::takePiece(qint8 i, qint8 j)
{
    untakePiece();
    m_takenPiece = {i, j};
    moveField(i, j);

    m_beatField = m_game.takePiece(i, j);

    auto chessBoard = m_game.getChessBoard();

    for (const auto &field : m_beatField) {
        if (chessBoard[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(m_chessBoardLabels[field.first][field.second]->size()
                                                                       * BEAT_FIELD_SIZE);
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces["beatField"]);
        } else {
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces[chessBoard[field.first][field.second] + "beatPiece"]);
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
        } else {
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces[chessBoard[field.first][field.second]]);
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
    QStringList pieceKeys = {"wK", "wQ", "wR", "wB", "wN", "wP", "bK", "bQ", "bR", "bB", "bN", "bP"};
    std::unordered_map<QString, QPixmap> pixmapOfPieces;

    for (const QString &key : pieceKeys)
        pixmapOfPieces[key] = QPixmap(QString(":/images/src/%1.png").arg(key));

    QSize baseSize = pixmapOfPieces.begin()->second.size();
    QPixmap overlay(":/images/src/beatPiece.png");
    overlay = overlay.scaled(baseSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int xPos = (baseSize.width() - overlay.width()) / 2;
    int yPos = (baseSize.height() - overlay.height()) / 2;

    for (const QString &key : pieceKeys) {
        QPixmap background = pixmapOfPieces[key];

        QPainter painter(&background);
        painter.setOpacity(BEAT_FIELD_OPACITY);
        painter.drawPixmap(xPos, yPos, overlay);

        m_imagesOfPieces[key] = QIcon(pixmapOfPieces[key]);
        m_imagesOfPieces[key + "beatPiece"] = QIcon(background);
    }

    QPixmap beatFieldPixmap(":/images/src/beatField.png");
    QPixmap transparentPixmap(beatFieldPixmap.size());
    transparentPixmap.fill(Qt::transparent);

    QPainter painter(&transparentPixmap);
    painter.setOpacity(BEAT_FIELD_OPACITY);
    painter.drawPixmap(0, 0, beatFieldPixmap);
    painter.end();

    m_imagesOfPieces["beatField"] = QIcon(transparentPixmap);
}

void MainWindow::updateChessScene()
{
    auto chessBoard = m_game.getChessBoard();
    for (qint8 i = 0; i < 8; ++i) {
        for (qint8 j = 0; j < 8; ++j) {
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());

            if (chessBoard[i][j].isEmpty())
                m_chessBoardLabels[i][j]->setIcon(QIcon());
            else
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
    chess.chess960 = false;

    std::vector<std::vector<QString>> chessBoard(8, std::vector<QString>(8));

    chessBoard[chess.posKings.first.first][chess.posKings.first.second] = "wK";
    chessBoard[0][3] = "wQ";
    chessBoard[chess.posRooksWhite.first.first][chess.posRooksWhite.first.second] = "wR";
    chessBoard[chess.posRooksWhite.second.first][chess.posRooksWhite.second.second] = "wR";
    chessBoard[0][2] = "wB";
    chessBoard[0][5] = "wB";
    chessBoard[0][6] = "wN";
    chessBoard[0][1] = "wN";

    chessBoard[chess.posKings.second.first][chess.posKings.second.second] = "bK";
    chessBoard[7][3] = "bQ";
    chessBoard[chess.posRooksBlack.first.first][chess.posRooksBlack.first.second] = "bR";
    chessBoard[chess.posRooksBlack.second.first][chess.posRooksBlack.second.second] = "bR";
    chessBoard[7][2] = "bB";
    chessBoard[7][5] = "bB";
    chessBoard[7][6] = "bN";
    chessBoard[7][1] = "bN";

    for (qint8 i = 0; i < 8; ++i) {
        chessBoard[1][i] = "wP";
        chessBoard[6][i] = "bP";
    }

    chess.chessBoard = chessBoard;

    return chess;
}
