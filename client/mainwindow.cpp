#define EMPTY 8
#define BEAT_FIELD_SIZE 0.3

#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setMinimumSize(QSize(405, 400));
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_beatField.reserve(27);
    m_imagesOfPieces.reserve(20);
    m_chessBoardLabels.resize(8, std::vector<QPushButton *>(8, nullptr));
    m_lastMove = {{EMPTY, 0}, {0, 0}};
    m_takenPiece = {EMPTY, 0};
    m_posKings = {{0, 4}, {7, 4}};

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

void MainWindow::clickField(const QString &nameField)
{
    int i = nameField[1].digitValue() - 1;
    int j = nameField[0].unicode() - 'a';

    if (m_takenPiece.first != EMPTY) {
        if (std::any_of(m_beatField.begin(), m_beatField.end(), [ = ](const auto & p) {
        return p.first == i && p.second == j;
    })) {
            if (m_lastMove.first.first != EMPTY) {
                uncheckField(m_lastMove.first.first, m_lastMove.first.second);
                uncheckField(m_lastMove.second.first, m_lastMove.second.second);
            }
            untakePiece();

            if (m_chessBoard[m_takenPiece.first][m_takenPiece.second][1] == 'P' && m_chessBoard[i][j].isEmpty()
                    && m_takenPiece.second != j) {
                m_chessBoard[m_takenPiece.first][j].clear();
                m_chessBoardLabels[m_takenPiece.first][j]->setIcon(QIcon());
            }

            if (m_chessBoard[m_takenPiece.first][m_takenPiece.second] == "wK")
                m_posKings.first = {m_takenPiece.first, m_takenPiece.second};
            else if (m_chessBoard[m_takenPiece.first][m_takenPiece.second] == "bK")
                m_posKings.second = {m_takenPiece.first, m_takenPiece.second};

            m_chessBoard[i][j] = m_chessBoard[m_takenPiece.first][m_takenPiece.second];
            m_chessBoardLabels[i][j]->setIcon(m_imagesOfPieces[m_chessBoard[i][j]]);
            checkField(i, j);
            m_whiteMove ^= true;

            m_lastMove = {{i, j}, {m_takenPiece.first, m_takenPiece.second}};

            m_chessBoard[m_takenPiece.first][m_takenPiece.second].clear();
            m_chessBoardLabels[m_takenPiece.first][m_takenPiece.second]->setIcon(QIcon());
            m_takenPiece.first = EMPTY;

            if (m_whiteMove) {
                uncheckField(m_posKings.second.first, m_posKings.second.second);
                if (isCheck(m_posKings.first.first, m_posKings.first.second, m_whiteMove)) {
                    m_chessBoardLabels[m_posKings.first.first][m_posKings.first.second]
                    ->setStyleSheet("background-color: #ff3838; border: none;");
                }
            } else {
                uncheckField(m_posKings.second.first, m_posKings.second.second);
                if (isCheck(m_posKings.second.first, m_posKings.second.second, m_whiteMove)) {
                    m_chessBoardLabels[m_posKings.second.first][m_posKings.second.second]
                    ->setStyleSheet("background-color: #ff3838; border: none;");
                }
            }
        } else if ((m_takenPiece.first != i || m_takenPiece.second != j)
                   && !m_chessBoard[i][j].isEmpty()
                   && ((m_whiteMove && m_chessBoard[i][j][0] == 'w') || (!m_whiteMove && m_chessBoard[i][j][0] == 'b'))) {
            uncheckField(m_takenPiece.first, m_takenPiece.second);
            takePiece(i, j);
        } else {
            if (m_chessBoard[m_takenPiece.first][m_takenPiece.second][1] == 'K'
                    && isCheck(m_posKings.second.first, m_posKings.second.second, m_whiteMove))
                m_chessBoardLabels[m_takenPiece.first][m_takenPiece.second]
                ->setStyleSheet("background-color: #ff3838; border: none;");
            else
                uncheckField(m_takenPiece.first, m_takenPiece.second);

            untakePiece();
            m_takenPiece.first = EMPTY;
        }
    } else {
        if (!m_chessBoard[i][j].isEmpty())
            if ((m_whiteMove && m_chessBoard[i][j][0] == 'w')
                    || (!m_whiteMove && m_chessBoard[i][j][0] == 'b'))
                takePiece(i, j);
    }
}

void MainWindow::takePiece(int i, int j)
{
    untakePiece();
    m_takenPiece = {i, j};
    checkField(i, j);

    const auto namePiece = m_chessBoard[i][j].toStdString();

    switch (namePiece[1]) {
    case 'K':
        for (int row = i - 1; row < i + 2; ++row)
            for (int col = j - 1; col < j + 2; ++col)
                if (checkMove(row, col) && !isCheck(row, col, m_whiteMove))
                    m_beatField.push_back({row, col});
        break;

    case 'Q':
        for (int j2 = j + 1; j2 < 8; ++j2) {
            if (checkMove(i, j2)) {
                m_beatField.push_back({i, j2});
                if (!m_chessBoard[i][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int j2 = j - 1; j2 >= 0; --j2) {
            if (checkMove(i, j2)) {
                m_beatField.push_back({i, j2});
                if (!m_chessBoard[i][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i + 1; i2 < 8; ++i2) {
            if (checkMove(i2, j)) {
                m_beatField.push_back({i2, j});
                if (!m_chessBoard[i2][j].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1; i2 >= 0; --i2) {
            if (checkMove(i2, j)) {
                m_beatField.push_back({i2, j});
                if (!m_chessBoard[i2][j].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8; ++i2, ++j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1, j2 = j - 1; i2 >= 0 && j2 >= 0; --i2, --j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1, j2 = j + 1; i2 >= 0 && j2 < 8; --i2, ++j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i + 1, j2 = j - 1; i2 < 8 && j2 >= 0; ++i2, --j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        break;

    case 'R':
        for (int j2 = j + 1; j2 < 8; ++j2) {
            if (checkMove(i, j2)) {
                m_beatField.push_back({i, j2});
                if (!m_chessBoard[i][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int j2 = j - 1; j2 >= 0; --j2) {
            if (checkMove(i, j2)) {
                m_beatField.push_back({i, j2});
                if (!m_chessBoard[i][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i + 1; i2 < 8; ++i2) {
            if (checkMove(i2, j)) {
                m_beatField.push_back({i2, j});
                if (!m_chessBoard[i2][j].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1; i2 >= 0; --i2) {
            if (checkMove(i2, j)) {
                m_beatField.push_back({i2, j});
                if (!m_chessBoard[i2][j].isEmpty())
                    break;
            } else {
                break;
            }
        }
        break;

    case 'B':
        for (int i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8; ++i2, ++j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1, j2 = j - 1; i2 >= 0 && j2 >= 0; --i2, --j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i - 1, j2 = j + 1; i2 >= 0 && j2 < 8; --i2, ++j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        for (int i2 = i + 1, j2 = j - 1; i2 < 8 && j2 >= 0; ++i2, --j2) {
            if (checkMove(i2, j2)) {
                m_beatField.push_back({i2, j2});
                if (!m_chessBoard[i2][j2].isEmpty())
                    break;
            } else {
                break;
            }
        }
        break;

    case 'N':
        if (checkMove(i + 2, j + 1))
            m_beatField.push_back({i + 2, j + 1});
        if (checkMove(i + 2, j - 1))
            m_beatField.push_back({i + 2, j - 1});
        if (checkMove(i - 2, j + 1))
            m_beatField.push_back({i - 2, j + 1});
        if (checkMove(i - 2, j - 1))
            m_beatField.push_back({i - 2, j - 1});
        if (checkMove(i + 1, j + 2))
            m_beatField.push_back({i + 1, j + 2});
        if (checkMove(i - 1, j + 2))
            m_beatField.push_back({i - 1, j + 2});
        if (checkMove(i + 1, j - 2))
            m_beatField.push_back({i + 1, j - 2});
        if (checkMove(i - 1, j - 2))
            m_beatField.push_back({i - 1, j - 2});
        break;

    default:
        if (namePiece[0] == 'w') {
            if (checkMove(i + 1, j))
                m_beatField.push_back({i + 1, j});
            if (i == 1 && checkMove(i + 2, j))
                m_beatField.push_back({i + 2, j});
            if (checkMove(i + 1, j + 1) && !m_chessBoard[i + 1][j + 1].isEmpty())
                m_beatField.push_back({i + 1, j + 1});
            if (checkMove(i + 1, j - 1) && !m_chessBoard[i + 1][j - 1].isEmpty())
                m_beatField.push_back({i + 1, j - 1});
            if (i == 4) {
                if (checkMove(i, j + 1) && m_chessBoard[i][j + 1] == "bP"
                        && m_lastMove.first == std::pair{i, j + 1}
                        && m_lastMove.second == std::pair{i + 2, j + 1})
                    m_beatField.push_back({i + 1, j + 1});
                if (checkMove(i, j - 1) && m_chessBoard[i][j - 1] == "bP"
                        && m_lastMove.first == std::pair{i, j - 1}
                        && m_lastMove.second == std::pair{i + 2, j - 1})
                    m_beatField.push_back({i + 1, j - 1});
            }
        } else {
            if (checkMove(i - 1, j))
                m_beatField.push_back({i - 1, j});
            if (i == 6 && checkMove(i - 2, j))
                m_beatField.push_back({i - 2, j});
            if (checkMove(i - 1, j + 1) && !m_chessBoard[i - 1][j + 1].isEmpty())
                m_beatField.push_back({i - 1, j + 1});
            if (checkMove(i - 1, j - 1) && !m_chessBoard[i - 1][j - 1].isEmpty())
                m_beatField.push_back({i - 1, j - 1});
            if (i == 3) {
                if (checkMove(i, j + 1) && m_chessBoard[i][j + 1] == "wP"
                        && m_lastMove.first == std::pair{i, j + 1}
                        && m_lastMove.second == std::pair{i - 2, j + 1})
                    m_beatField.push_back({i - 1, j + 1});
                if (checkMove(i, j - 1) && m_chessBoard[i][j - 1] == "wP"
                        && m_lastMove.first == std::pair{i, j - 1}
                        && m_lastMove.second == std::pair{i - 2, j - 1})
                    m_beatField.push_back({i - 1, j - 1});
            }
        }
        break;
    }

    for (const auto &field : m_beatField) {
        if (m_chessBoard[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(
                m_chessBoardLabels[field.first][field.second]->size() * BEAT_FIELD_SIZE);
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces["beatField"]);
        }
    }
}

void MainWindow::untakePiece()
{
    for (const auto &field : m_beatField) {
        if (m_chessBoard[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(m_chessBoardLabels[field.first][field.second]->size());
            m_chessBoardLabels[field.first][field.second]->setIcon(QIcon());
        }
    }
    m_beatField.clear();
}

bool MainWindow::checkMove(int i, int j)
{
    return i >= 0 && i < 8 && j >= 0 && j < 8
           && (m_chessBoard[i][j].isEmpty() || (m_whiteMove && m_chessBoard[i][j][0] == 'b')
               || (!m_whiteMove && m_chessBoard[i][j][0] == 'w'));
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

bool MainWindow::isCheck(int i, int j, bool white)
{
    for (int j2 = j + 1; j2 < 8; ++j2) {
        if (!m_chessBoard[i][j2].isEmpty()) {
            if ((white && m_chessBoard[i][j2][0] == 'b')
                    || (!white && m_chessBoard[i][j2][0] == 'w')) {
                if (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q'
                        || (m_chessBoard[i][j2][1] == 'K' && j2 == (j + 1)))
                    return true;
            }
            break;
        }
    }
    for (int j2 = j - 1; j2 >= 0; --j2) {
        if (!m_chessBoard[i][j2].isEmpty()) {
            if ((white && m_chessBoard[i][j2][0] == 'b')
                    || (!white && m_chessBoard[i][j2][0] == 'w')) {
                if (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q'
                        || (m_chessBoard[i][j2][1] == 'K' && j2 == (j - 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i + 1; i2 < 8; ++i2) {
        if (!m_chessBoard[i2][j].isEmpty()) {
            if ((white && m_chessBoard[i2][j][0] == 'b')
                    || (!white && m_chessBoard[i2][j][0] == 'w')) {
                if (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q'
                        || (m_chessBoard[i2][j][1] == 'K' && i2 == (i + 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i - 1; i2 >= 0; --i2) {
        if (!m_chessBoard[i2][j].isEmpty()) {
            if ((white && m_chessBoard[i2][j][0] == 'b')
                    || (!white && m_chessBoard[i2][j][0] == 'w')) {
                if (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q'
                        || (m_chessBoard[i2][j][1] == 'K' && i2 == (i + 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8; ++i2, ++j2) {
        if (!m_chessBoard[i2][j2].isEmpty()) {
            if ((white && m_chessBoard[i2][j2][0] == 'b')
                    || (!white && m_chessBoard[i2][j2][0] == 'w')) {
                if (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q'
                        || ((m_chessBoard[i2][j2][1] == 'K' || m_chessBoard[i2][j2][1] == 'P')
                            && i2 == (i + 1) && j2 == (j + 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i - 1, j2 = j - 1; i2 >= 0 && j2 >= 0; --i2, --j2) {
        if (!m_chessBoard[i2][j2].isEmpty()) {
            if ((white && m_chessBoard[i2][j2][0] == 'b')
                    || (!white && m_chessBoard[i2][j2][0] == 'w')) {
                if (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q'
                        || ((m_chessBoard[i2][j2][1] == 'K' || m_chessBoard[i2][j2][1] == 'P')
                            && i2 == (i - 1) && j2 == (j - 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i - 1, j2 = j + 1; i2 >= 0 && j2 < 8; --i2, ++j2) {
        if (!m_chessBoard[i2][j2].isEmpty()) {
            if ((white && m_chessBoard[i2][j2][0] == 'b')
                    || (!white && m_chessBoard[i2][j2][0] == 'w')) {
                if (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q'
                        || ((m_chessBoard[i2][j2][1] == 'K' || m_chessBoard[i2][j2][1] == 'P')
                            && i2 == (i - 1) && j2 == (j + 1)))
                    return true;
            }
            break;
        }
    }
    for (int i2 = i + 1, j2 = j - 1; i2 < 8 && j2 >= 0; ++i2, --j2) {
        if (!m_chessBoard[i2][j2].isEmpty()) {
            if ((white && m_chessBoard[i2][j2][0] == 'b')
                    || (!white && m_chessBoard[i2][j2][0] == 'w')) {
                if (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q'
                        || ((m_chessBoard[i2][j2][1] == 'K' || m_chessBoard[i2][j2][1] == 'P')
                            && i2 == (i + 1) && j2 == (j - 1)))
                    return true;
            }
            break;
        }
    }
    if ((i + 2) < 8
            && (j + 1) < 8
            && ((white && m_chessBoard[i + 2][j + 1] == "bN")
                || (!white && m_chessBoard[i + 2][j + 1] == "wN")))
        return true;
    if ((i + 2) < 8
            && (j - 1) >= 0
            && ((white && m_chessBoard[i + 2][j - 1] == "bN")
                || (!white && m_chessBoard[i + 2][j - 1] == "wN")))
        return true;
    if ((i - 2) >= 0
            && (j + 1) < 8
            && ((white && m_chessBoard[i - 2][j + 1] == "bN")
                || (!white && m_chessBoard[i - 2][j + 1] == "wN")))
        return true;
    if ((i - 2) >= 0
            && (j - 1) >= 0
            && ((white && m_chessBoard[i - 2][j - 1] == "bN")
                || (!white && m_chessBoard[i - 2][j - 1] == "wN")))
        return true;
    if ((i + 1) < 8
            && (j + 2) < 8
            && ((white && m_chessBoard[i + 1][j + 2] == "bN")
                || (!white && m_chessBoard[i + 1][j + 2] == "wN")))
        return true;
    if ((i - 1) >= 0
            && (j + 2) < 8
            && ((white && m_chessBoard[i - 1][j + 2] == "bN")
                || (!white && m_chessBoard[i - 1][j + 2] == "wN")))
        return true;
    if ((i + 1) < 8
            && (j - 2) >= 0
            && ((white && m_chessBoard[i + 1][j + 2] == "bN")
                || (!white && m_chessBoard[i + 1][j + 2] == "wN")))
        return true;
    if ((i - 1) >= 0
            && (j - 2) >= 0
            && ((white && m_chessBoard[i - 1][j + 2] == "bN")
                || (!white && m_chessBoard[i - 1][j + 2] == "wN")))
        return true;

    return false;
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
    m_imagesOfPieces["beatField"] = QIcon(":/images/src/beatField.png");
}

void MainWindow::fillStandartChessBoard()
{
    m_chessBoard.resize(8, std::vector<QString>(8));

    m_chessBoard[0][4] = "wK";
    m_chessBoard[0][3] = "wQ";
    m_chessBoard[0][0] = "wR";
    m_chessBoard[0][7] = "wR";
    m_chessBoard[0][2] = "wB";
    m_chessBoard[0][5] = "wB";
    m_chessBoard[0][1] = "wN";
    m_chessBoard[0][6] = "wN";

    m_chessBoard[7][3] = "bQ";
    m_chessBoard[7][4] = "bK";
    m_chessBoard[7][0] = "bR";
    m_chessBoard[7][7] = "bR";
    m_chessBoard[7][2] = "bB";
    m_chessBoard[7][5] = "bB";
    m_chessBoard[7][1] = "bN";
    m_chessBoard[7][6] = "bN";

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
