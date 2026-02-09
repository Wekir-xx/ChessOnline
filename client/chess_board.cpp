#include "chess_board.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
{
    m_imagesOfPieces.reserve(25);
    m_chessBoardLabels.resize(8, std::vector<QPushButton *>(8, nullptr));

    fillIcan();

    m_game.setChessParams(fillStandartChessBoard());

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);

    QHBoxLayout *chess_board_layout = new QHBoxLayout();
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
        QLabel *label = new QLabel();
        label->setMinimumHeight(MINIMUM_PIECE_SIZE);
        label->setFixedWidth(20);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString::number(8 - i));
        row_layout->addWidget(label);
    }

    QLabel *empty = new QLabel();
    empty->setSizePolicy(sizePolicy);
    empty->setFixedHeight(20);
    row_layout->addWidget(empty);

    for (qint8 i = 0; i < 8; ++i) {
        QLabel *label = new QLabel();
        label->setMinimumWidth(MINIMUM_PIECE_SIZE);
        label->setFixedHeight(20);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString(QChar('a' + i)));
        name_fields->addWidget(label);
    }

    for (qint8 i = 0; i < 8; ++i) {
        QVBoxLayout *layout = new QVBoxLayout();
        layout->setSpacing(0);

        for (qint8 j = 0; j < 8; ++j) {
            QPushButton *button = new QPushButton();
            button->setObjectName(QString(QChar('a' + i)) + QString::number(8 - j));
            button->setSizePolicy(sizePolicy);
            button->setMinimumSize(MINIMUM_PIECE_SIZE, MINIMUM_PIECE_SIZE);

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

    this->setLayout(chess_board_layout);
}

void ChessBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateChessScene();
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    for (qint8 i = 0; i < 8; ++i)
        for (qint8 j = 0; j < 8; ++j)
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());

    const auto &beatField = m_game.getBeatFields();
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatField)
        if (chessFields[field.first][field.second].isEmpty())
            m_chessBoardLabels[field.first][field.second]->setIconSize(m_chessBoardLabels[field.first][field.second]->size()
                                                                       * BEAT_FIELD_SIZE);
}

void ChessBoard::clickField(const QString &nameField)
{
    qint8 i = nameField[1].digitValue() - 1;
    qint8 j = nameField[0].unicode() - 'a';

    bool whiteMove = m_game.getColorMove();

    QChar color = 'w';
    if (!whiteMove)
        color = 'b';

    const auto &chessFields = m_game.getChessFields();
    const auto &beatFields = m_game.getBeatFields();
    const auto &takenPiece = m_game.getTakenPiece();

    if (takenPiece.first != EMPTY) {
        if (std::any_of(beatFields.begin(), beatFields.end(), [=](const auto &p) { return p.first == i && p.second == j; })) {
            untakePiece();

            if (!m_transformPawn && chessFields[takenPiece.first][takenPiece.second][1] == 'P' && (i == 0 || i == 7)) {
                m_transformPawn = true;
                m_game.chooseTransformPawn(j);
                transformPawnField(beatFields);
            } else {
                if (m_transformPawn) {
                    m_transformPawn = false;
                    m_game.transformPawn(i, j);
                    untransformPawnField(beatFields);

                    if (whiteMove)
                        i = 7;
                    else
                        i = 0;
                }

                const auto &lastMove = m_game.getLastMove();
                if (lastMove.first.first != EMPTY) {
                    baseField(lastMove.first.first, lastMove.first.second);
                    baseField(lastMove.second.first, lastMove.second.second);
                }

                const auto &posKings = m_game.getPosKings();
                if (m_game.getCheck() && chessFields[takenPiece.first][takenPiece.second][1] != 'K') {
                    if (whiteMove)
                        baseField(posKings.first.first, posKings.first.second);
                    else
                        baseField(posKings.second.first, posKings.second.second);
                }

                m_game.movePiece(std::pair{i, j});
                moveField(lastMove.second.first, lastMove.second.second);

                if (!whiteMove) {
                    if (m_game.getCheck()) {
                        checkField(posKings.first.first, posKings.first.second);

                        if (!m_game.isPossibleMove())
                            this->setEnabled(false);
                    } else if (!m_game.isPossibleMove()) {
                        this->setEnabled(false);
                    }
                } else {
                    if (m_game.getCheck()) {
                        checkField(posKings.second.first, posKings.second.second);

                        if (!m_game.isPossibleMove())
                            this->setEnabled(false);
                    } else if (!m_game.isPossibleMove()) {
                        this->setEnabled(false);
                    }
                }
            }

            updateChessScene();
        } else {
            if (m_transformPawn) {
                m_transformPawn = false;
                m_game.untransformPawn();
                untransformPawnField(beatFields);
            }

            if (chessFields[takenPiece.first][takenPiece.second][1] == 'K' && m_game.getCheck())
                checkField(takenPiece.first, takenPiece.second);
            else
                baseField(takenPiece.first, takenPiece.second);

            if ((takenPiece.first != i || takenPiece.second != j) && !chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
                takePiece(i, j);
            } else {
                untakePiece();
                m_game.untakePiece();
            }
        }
    } else if (!chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
        takePiece(i, j);
    }
}

void ChessBoard::checkField(qint8 i, qint8 j)
{
    m_chessBoardLabels[i][j]->setStyleSheet("background-color: #ff3838; border: none;");
}

void ChessBoard::moveField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #B9CA43; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #F5F682; border: none;");
}

void ChessBoard::baseField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #739552; border: none;");
    else
        m_chessBoardLabels[i][j]->setStyleSheet("background-color: #EBECD0; border: none;");
}

void ChessBoard::takePiece(qint8 i, qint8 j)
{
    untakePiece();
    moveField(i, j);

    const auto &beatField = m_game.takePiece(i, j);
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatField) {
        if (chessFields[field.first][field.second].isEmpty()) {
            m_chessBoardLabels[field.first][field.second]->setIconSize(m_chessBoardLabels[field.first][field.second]->size()
                                                                       * BEAT_FIELD_SIZE);
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces["beatField"]);
        } else {
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces[chessFields[field.first][field.second] + "beatPiece"]);
        }
    }
}

void ChessBoard::untakePiece()
{
    const auto &beatFields = m_game.getBeatFields();
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatFields) {
        if (chessFields[field.first][field.second].isEmpty())
            m_chessBoardLabels[field.first][field.second]->setIcon(QIcon());
        else
            m_chessBoardLabels[field.first][field.second]->setIcon(m_imagesOfPieces[chessFields[field.first][field.second]]);
    }
}

void ChessBoard::transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        m_chessBoardLabels[field.first][field.second]->setStyleSheet("background-color: #FFFFFF; border: none;");
}

void ChessBoard::untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        baseField(field.first, field.second);
}

void ChessBoard::fillIcan()
{
    QStringList pieceKeys = {"wK", "wQ", "wR", "wB", "wN", "wP", "bK", "bQ", "bR", "bB", "bN", "bP"};
    std::unordered_map<QString, QPixmap> pixmapOfPieces;

    for (const QString &key : pieceKeys)
        pixmapOfPieces[key] = QPixmap(path + QString("%1.png").arg(key));

    QSize baseSize = pixmapOfPieces.begin()->second.size();
    QPixmap overlay(path + "beatPiece.png");
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

    QPixmap beatFieldPixmap(path + "beatField.png");
    QPixmap transparentPixmap(beatFieldPixmap.size());
    transparentPixmap.fill(Qt::transparent);

    QPainter painter(&transparentPixmap);
    painter.setOpacity(BEAT_FIELD_OPACITY);
    painter.drawPixmap(0, 0, beatFieldPixmap);
    painter.end();

    m_imagesOfPieces["beatField"] = QIcon(transparentPixmap);
}

void ChessBoard::updateChessScene()
{
    const auto &chessFields = m_game.getChessFields();
    for (qint8 i = 0; i < 8; ++i) {
        for (qint8 j = 0; j < 8; ++j) {
            m_chessBoardLabels[i][j]->setIconSize(m_chessBoardLabels[i][j]->size());

            if (chessFields[i][j].isEmpty())
                m_chessBoardLabels[i][j]->setIcon(QIcon());
            else
                m_chessBoardLabels[i][j]->setIcon(m_imagesOfPieces[chessFields[i][j]]);
        }
    }
}

ChessParams ChessBoard::fillStandartChessBoard()
{
    ChessParams chess;
    chess.posKings = {{0, 4}, {7, 4}};
    chess.posRooksWhite = {{0, 0}, {0, 7}};
    chess.posRooksBlack = {{7, 0}, {7, 7}};
    chess.chess960 = false;

    std::vector<std::vector<QString>> chessFields(8, std::vector<QString>(8));

    chessFields[chess.posKings.first.first][chess.posKings.first.second] = "wK";
    chessFields[0][3] = "wQ";
    chessFields[chess.posRooksWhite.first.first][chess.posRooksWhite.first.second] = "wR";
    chessFields[chess.posRooksWhite.second.first][chess.posRooksWhite.second.second] = "wR";
    chessFields[0][2] = "wB";
    chessFields[0][5] = "wB";
    chessFields[0][6] = "wN";
    chessFields[0][1] = "wN";

    chessFields[chess.posKings.second.first][chess.posKings.second.second] = "bK";
    chessFields[7][3] = "bQ";
    chessFields[chess.posRooksBlack.first.first][chess.posRooksBlack.first.second] = "bR";
    chessFields[chess.posRooksBlack.second.first][chess.posRooksBlack.second.second] = "bR";
    chessFields[7][2] = "bB";
    chessFields[7][5] = "bB";
    chessFields[7][6] = "bN";
    chessFields[7][1] = "bN";

    for (qint8 i = 0; i < 8; ++i) {
        chessFields[1][i] = "wP";
        chessFields[6][i] = "bP";
    }

    chess.chessFields = chessFields;

    return chess;
}
