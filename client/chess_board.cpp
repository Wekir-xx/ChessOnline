#include "chess_board.h"

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    m_imagesOfPieces.reserve(40);
    m_chessBoardBut.resize(8, std::vector<EventButton *>(8, nullptr));
    m_otherBoardLab.resize(2, std::vector<QLabel *>(8, nullptr));

#ifdef MOVE_PIECE
    m_movePiece = new QLabel(this);
    m_movePiece->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_movePiece->setWindowFlags(Qt::FramelessWindowHint);
    m_movePiece->hide();
#endif

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);

    m_board = new QGridLayout();
    m_board->setSpacing(0);

    QLabel *empty = new QLabel();
    empty->setSizePolicy(sizePolicy);
    empty->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_board->addWidget(empty, 8, 0);

    for (qint8 i = 0; i < 8; ++i) {
        QLabel *label = new QLabel();
        label->setMinimumHeight(MINIMUM_PIECE_SIZE);
        label->setFixedWidth(FIXED_SIZE_NUMBERS);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString::number(8 - i));
        m_otherBoardLab[0][i] = label;

        label = new QLabel();
        label->setMinimumWidth(MINIMUM_PIECE_SIZE);
        label->setFixedHeight(FIXED_SIZE_NUMBERS);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString(QChar('a' + i)));
        m_otherBoardLab[1][i] = label;

        for (qint8 j = 0; j < 8; ++j) {
            EventButton *button = new EventButton();
            button->setObjectName(QString(QChar('a' + j)) + QString::number(i + 1));
            button->setSizePolicy(sizePolicy);
            button->setMinimumSize(MINIMUM_PIECE_SIZE, MINIMUM_PIECE_SIZE);

            m_chessBoardBut[i][j] = button;
            this->baseField(i, j);

            connect(button, &EventButton::pressMouseSignal, this, [this, button]() {
                if (!(m_blockBoard || m_blockBoardHistory))
                    this->clickField(button->objectName());
            });
        }
    }

    this->fillFullIcans();
    this->fillBoard();
    this->setLayout(m_board);
}

void ChessBoard::resetBoard()
{
    this->updateChessScene();
    for (qint8 i = 0; i < 8; ++i)
        for (qint8 j = 0; j < 8; ++j)
            this->baseField(i, j);
}

void ChessBoard::fillStandartChessBoard()
{
    ChessGame::ChessParams chess;
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
    m_game.setChessParams(chess, {{true, true}, {true, true}});
}

void ChessBoard::fillStandart960ChessBoard()
{
    ChessGame::ChessParams chess;
    chess.chess960 = true;

    std::vector<std::vector<QString>> chessFields(8, std::vector<QString>(8));

    std::vector<qint8> numPos{0, 1, 2, 3, 4, 5, 6, 7};
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> randomBishop(1, 4);
    qint8 firstBishop = randomBishop(gen) * 2 - 2;
    qint8 secondBishop = randomBishop(gen) * 2 - 1;

    chessFields[0][firstBishop] = "wB";
    chessFields[0][secondBishop] = "wB";
    chessFields[7][firstBishop] = "bB";
    chessFields[7][secondBishop] = "bB";

    auto it = std::find(v.begin(), v.end(), firstBishop);
    v.erase(it);
    it = std::find(v.begin(), v.end(), secondBishop);
    v.erase(it);

    std::uniform_int_distribution<> randomQueen(0, 5);
    qint8 queen = randomQueen(gen);

    chessFields[0][numPos[queen]] = "wQ";
    chessFields[7][numPos[queen]] = "bQ";

    it = std::find(v.begin(), v.end(), queen);
    v.erase(it);

    for (qint8 i = 0; i < 8; ++i) {
        chessFields[1][i] = "wP";
        chessFields[6][i] = "bP";
    }

    chess.chessFields = chessFields;
    m_game.setChessParams(chess, {{true, true}, {true, true}});
}

void ChessBoard::fillUserChessBoard(std::vector<std::vector<QString>> chessFields,
                                    bool chess960,
                                    std::pair<std::pair<bool, bool>, std::pair<bool, bool>> castling)
{
    ChessGame::ChessParams chess;
    chess.chess960 = chess960;
    chess.chessFields = chessFields;
    chess.posRooksWhite = {{EMPTY, EMPTY}, {EMPTY, EMPTY}};
    chess.posRooksBlack = {{EMPTY, EMPTY}, {EMPTY, EMPTY}};

    for (size_t i = 0; i < 8; ++i) {
        for (size_t j = 0; j < 8; ++j) {
            if (chessFields[i][j] == "wK") {
                chess.posKings.first = {i, j};
            } else if (chessFields[i][j] == "bK") {
                chess.posKings.second = {i, j};
            } else if (chessFields[i][j] == "bK") {
                if (chess.posRooksWhite.first.first == EMPTY)
                    chess.posRooksWhite.first = {i, j};
                else
                    chess.posRooksWhite.second = {i, j};
            } else if (chessFields[i][j] == "bK") {
                if (chess.posRooksBlack.first.first == EMPTY)
                    chess.posRooksBlack.first = {i, j};
                else
                    chess.posRooksBlack.second = {i, j};
            }
        }
    }

    m_game.setChessParams(chess, castling);
}

void ChessBoard::turnBoard()
{
    if (m_turnChess) {
        fillIcan(m_turnBoard, true);
        fillIcan(!m_turnBoard, false);
        this->updateChessScene();
    }

    m_turnBoard ^= true;
    this->fillBoard();
}

void ChessBoard::turnChess()
{
    m_turnChess ^= true;
    if (m_turnBoard)
        fillIcan(true, !m_turnChess);
    else
        fillIcan(false, !m_turnChess);

    this->updateChessScene();
}

void ChessBoard::historyBack()
{
    if (m_game.isPossibleHistoryBack()) {
        m_blockBoardHistory = true;
        updateHistoryScene();
    }
}

void ChessBoard::historyForward()
{
    if (m_game.isPossibleHistoryForward()) {
        m_blockBoardHistory = false;
        updateHistoryScene();
    }
}

void ChessBoard::setBlockBoard(bool flag)
{
    m_blockBoard = flag;
}

void ChessBoard::setAutoQueen(bool flag)
{
    m_autoQueen = flag;
}

void ChessBoard::setPremove(bool flag)
{
    m_premove = flag;
}

bool ChessBoard::getBlockBoard()
{
    return m_blockBoard;
}

bool ChessBoard::getColorMove()
{
    return m_game.getColorMove();
}

void ChessBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->updateChessScene();
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    for (qint8 i = 0; i < 8; ++i)
        for (qint8 j = 0; j < 8; ++j)
            m_chessBoardBut[i][j]->setIconSize(m_chessBoardBut[i][j]->size());

    const auto &beatField = m_game.getBeatFields();
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatField)
        if (chessFields[field.first][field.second].isEmpty())
            m_chessBoardBut[field.first][field.second]->setIconSize(m_chessBoardBut[field.first][field.second]->size()
                    * BEAT_FIELD_SIZE);
}

#ifdef MOVE_PIECE
void ChessBoard::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (m_takenPiece) {
        m_takenPiece = false;
        const auto takenPiece = m_game.getTakenPiece();
        const auto &chessFields = m_game.getChessFields();
        m_chessBoardBut[takenPiece.first][takenPiece.second]->setIcon(m_imagesOfPieces[chessFields[takenPiece.first][takenPiece.second]]);
        m_movePiece->hide();
    }
}

void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        const auto takenPiece = m_game.getTakenPiece();
        if (takenPiece.first != EMPTY) {
            if (!m_takenPiece) {
                m_takenPiece = true;
                const auto &chessFields = m_game.getChessFields();
                m_chessBoardBut[takenPiece.first][takenPiece.second]->setIcon(QIcon());
                QIcon piece = m_imagesOfPieces[chessFields[takenPiece.first][takenPiece.second]];
                m_movePiece->setPixmap(piece.pixmap(m_chessBoardBut[0][0]->size()));
                m_movePiece->setFixedSize(m_chessBoardBut[0][0]->size());
                m_movePiece->show();
                m_movePiece->raise();
            }
            QPoint pos = event->pos();
            m_movePiece->move(pos.x() - m_movePiece->width() / 2, pos.y() - m_movePiece->height() / 2);
        }
    }

    QWidget::mouseMoveEvent(event);
}
#endif

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
    const auto takenPiece = m_game.getTakenPiece();

    if (takenPiece.first != EMPTY) {
        if (std::any_of(beatFields.begin(), beatFields.end(), [ = ](const auto & p) {
        return p.first == i && p.second == j;
    })) {
            this->untakePiece();

            if (!m_transformPawn && chessFields[takenPiece.first][takenPiece.second][1] == 'P' && (i == 0 || i == 7)) {
                if (m_autoQueen) {
                    m_game.setField(color + 'Q', takenPiece.first, takenPiece.second);
                } else {
                    m_transformPawn = true;
                    m_game.chooseTransformPawn(j);
                    this->transformPawnField(beatFields);
                    this->updateChessScene();
                    return;
                }
            } else if (m_transformPawn) {
                m_transformPawn = false;
                m_game.transformPawn(i, j);
                this->untransformPawnField(beatFields);

                if (whiteMove)
                    i = 7;
                else
                    i = 0;
            }

            uncheckLastMove();
            uncheckKing();

            m_game.movePiece(std::pair{i, j});

            const auto lastMove = m_game.getLastMove();
            this->moveField(lastMove.second.first, lastMove.second.second);
            checkKing();

            if (!m_game.isPossibleMove()) {
                if (m_game.getCheck())
                    emit endGame(whiteMove ? ResultGame::WIN_WHITE : ResultGame::WIN_BLACK);
                else
                    emit endGame(ResultGame::STALE_MATE);
            } else if (m_game.isStaleMate()) {
                emit endGame(ResultGame::STALE_MATE);
            }

            this->updateChessScene();
            emit didMove();
        } else {
            if (m_transformPawn) {
                m_transformPawn = false;
                m_game.untransformPawn();
                this->untransformPawnField(beatFields);
            }

            if (chessFields[takenPiece.first][takenPiece.second][1] == 'K')
                checkKing();
            else
                this->baseField(takenPiece.first, takenPiece.second);

            if ((takenPiece.first != i || takenPiece.second != j) && !chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
                this->takePiece(i, j);
            } else {
                this->untakePiece();
                m_game.untakePiece();
            }
        }
    } else if (!chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
        this->takePiece(i, j);
    }
}

void ChessBoard::checkField(qint8 i, qint8 j)
{
    m_chessBoardBut[i][j]->setStyleSheet("background-color: #ff3838; border: none;");
}

void ChessBoard::moveField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #B9CA43; border: none;");
    else
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #F5F682; border: none;");
}

void ChessBoard::baseField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #739552; border: none;");
    else
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #EBECD0; border: none;");
}

void ChessBoard::uncheckLastMove()
{
    const auto lastMove = m_game.getLastMove();
    if (lastMove.first.first != EMPTY)
        this->baseField(lastMove.first.first, lastMove.first.second);
    if (lastMove.second.first != EMPTY)
        this->baseField(lastMove.second.first, lastMove.second.second);
}

void ChessBoard::checkKing()
{
    bool whiteMove = m_game.getColorMove();
    const auto posKings = m_game.getPosKings();
    if (m_game.getCheck()) {
        if (whiteMove)
            this->checkField(posKings.first.first, posKings.first.second);
        else
            this->checkField(posKings.second.first, posKings.second.second);
    }
}

void ChessBoard::uncheckKing()
{
    bool whiteMove = m_game.getColorMove();
    const auto posKings = m_game.getPosKings();
    if (m_game.getCheck()) {
        if (whiteMove)
            this->baseField(posKings.first.first, posKings.first.second);
        else
            this->baseField(posKings.second.first, posKings.second.second);
    }
}

void ChessBoard::takePiece(qint8 i, qint8 j)
{
    this->untakePiece();
    this->moveField(i, j);

    m_game.takePiece(i, j);
    const auto &beatFields = m_game.getBeatFields();
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatFields) {
        if (chessFields[field.first][field.second].isEmpty()) {
            m_chessBoardBut[field.first][field.second]->setIconSize(m_chessBoardBut[field.first][field.second]->size()
                    * BEAT_FIELD_SIZE);
            m_chessBoardBut[field.first][field.second]->setIcon(m_imagesOfPieces["beatField"]);
        } else {
            m_chessBoardBut[field.first][field.second]->setIcon(m_imagesOfPieces[chessFields[field.first][field.second] + "beatPiece"]);
        }
    }
}

void ChessBoard::untakePiece()
{
    const auto &beatFields = m_game.getBeatFields();
    const auto &chessFields = m_game.getChessFields();

    for (const auto &field : beatFields) {
        if (chessFields[field.first][field.second].isEmpty())
            m_chessBoardBut[field.first][field.second]->setIcon(QIcon());
        else
            m_chessBoardBut[field.first][field.second]->setIcon(m_imagesOfPieces[chessFields[field.first][field.second]]);
    }
}

void ChessBoard::transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        m_chessBoardBut[field.first][field.second]->setStyleSheet("background-color: #FFFFFF; border: none;");
}

void ChessBoard::untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        this->baseField(field.first, field.second);
}

void ChessBoard::fillFullIcans()
{
    fillIcan(true, true);
    fillIcan(false, true);

    QPixmap beatFieldPixmap(pathGeneral + "beatField.png");
    QPixmap transparentPixmap(beatFieldPixmap.size());
    transparentPixmap.fill(Qt::transparent);

    QPainter painter(&transparentPixmap);
    painter.setOpacity(BEAT_FIELD_OPACITY);
    painter.drawPixmap(0, 0, beatFieldPixmap);
    painter.end();

    m_imagesOfPieces["beatField"] = QIcon(transparentPixmap);
}

void ChessBoard::fillIcan(bool white, bool up)
{
    QChar color;
    if (white)
        color = 'w';
    else
        color = 'b';

    std::vector<QString> pieceKeys = {color + 'K', color + 'Q', color + 'R', color + 'B', color + 'N', color + 'P'};
    std::unordered_map<QString, QPixmap> pixmapOfPieces;

    QTransform transform;
    transform.rotate(180);
    for (const QString &key : pieceKeys) {
        QPixmap pix = QPixmap(pathStyle1 + QString("%1.png").arg(key));
        if (up)
            pixmapOfPieces[key] = pix;
        else
            pixmapOfPieces[key] = pix.transformed(transform);
    }

    QSize baseSize = pixmapOfPieces.begin()->second.size();
    QPixmap overlay(pathGeneral + "beatPiece.png");
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
}

void ChessBoard::fillBoard()
{
    if (m_turnBoard) {
        for (qint8 i = 0; i < 8; ++i) {
            m_board->addWidget(m_otherBoardLab[0][7 - i], i, 0);
            m_board->addWidget(m_otherBoardLab[1][7 - i], 8, i + 1);

            for (qint8 j = 0; j < 8; ++j)
                m_board->addWidget(m_chessBoardBut[i][7 - j], i, j + 1);
        }
    } else {
        for (qint8 i = 0; i < 8; ++i) {
            m_board->addWidget(m_otherBoardLab[0][i], i, 0);
            m_board->addWidget(m_otherBoardLab[1][i], 8, i + 1);

            for (qint8 j = 0; j < 8; ++j)
                m_board->addWidget(m_chessBoardBut[7 - i][j], i, j + 1);
        }
    }
}

void ChessBoard::updateChessScene()
{
    const auto &chessFields = m_game.getChessFields();
    for (qint8 i = 0; i < 8; ++i) {
        for (qint8 j = 0; j < 8; ++j) {
            m_chessBoardBut[i][j]->setIconSize(m_chessBoardBut[i][j]->size());

            if (chessFields[i][j].isEmpty())
                m_chessBoardBut[i][j]->setIcon(QIcon());
            else
                m_chessBoardBut[i][j]->setIcon(m_imagesOfPieces[chessFields[i][j]]);
        }
    }
}

void ChessBoard::updateHistoryScene()
{
    uncheckLastMove();
    uncheckKing();
    m_game.historyMove();
    const auto lastMove = m_game.getLastMove();
    if (lastMove.first.first != EMPTY)
        this->moveField(lastMove.first.first, lastMove.first.second);
    if (lastMove.second.first != EMPTY)
        this->moveField(lastMove.second.first, lastMove.second.second);
    checkKing();
    this->updateChessScene();
}
