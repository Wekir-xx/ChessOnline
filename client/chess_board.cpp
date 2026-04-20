#include "chess_board.h"

#include <QIcon>
#include <QChar>

ChessBoard::ChessBoard(StyleLib *styleLib, QWidget *parent)
    : QWidget(parent)
{
#ifdef MOVE_PIECE
    this->setMouseTracking(true);
    m_movePiece = new QLabel(this);
    m_movePiece->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_movePiece->setWindowFlags(Qt::FramelessWindowHint);
    m_movePiece->hide();
#endif

    m_chessBoard = new ChessBoardWidget(styleLib);

    m_mainLayout = new QVBoxLayout();
    m_mainLayout->addWidget(m_chessBoard);

    connect(m_chessBoard, &ChessBoardWidget::clickField, this, [this](QString field) {
        if (!(m_blockBoard || m_blockBoardHistory))
            this->clickField(field);
    });

    this->setLayout(m_mainLayout);
}

void ChessBoard::updateBoard()
{
    this->updateBoardIcon();
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->baseField(i, j);
}

void ChessBoard::fillChessBoard(bool chess960)
{
    ChessBoardParams boardParams;
    boardParams.chessFields.resize(SIDE_SIZE, std::vector<QString>(SIDE_SIZE));
    boardParams.castling = {{true, true}, {true, true}};
    boardParams.chess960 = chess960;
    boardParams.whiteMove = true;

    if (chess960)
        SomeConstans::fill960ChessField(boardParams.chessFields);
    else
        SomeConstans::fillStandartChessField(boardParams.chessFields);

    m_game.setChessParams(boardParams);
}

void ChessBoard::fillUserChessBoard(ChessBoardParams &boardParams)
{
    m_game.setChessParams(boardParams);
}

void ChessBoard::turnBoard()
{
    m_chessBoard->turnBoard();
    if (m_chessBoard->getTurnSecondPlayer())
        this->updateBoardIcon();
}

void ChessBoard::turnSecondPlayer()
{
    m_chessBoard->turnSecondPlayer();
    this->updateBoardIcon();
}

void ChessBoard::historyBack()
{
    if (m_game.isPossibleHistoryBack()) {
        m_blockBoardHistory = true;
        this->updateHistoryScene();
    }
}

void ChessBoard::historyForward()
{
    if (m_game.isPossibleHistoryForward()) {
        m_blockBoardHistory = false;
        this->updateHistoryScene();
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
    return m_whiteMove;
}

bool ChessBoard::getTurnBoard()
{
    return m_chessBoard->getTurnBoard();
}

bool ChessBoard::getTurnSecondPlayer()
{
    return m_chessBoard->getTurnSecondPlayer();
}

void ChessBoard::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->updateBoardIcon();
    this->updateBoardSize();
}

void ChessBoard::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->updateBoardSize();
}

#ifdef MOVE_PIECE
void ChessBoard::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    if (m_takenPiece) {
        m_takenPiece = false;
        const auto takenPiece = m_game.getTakenPiece();
        const auto &chessFields = m_game.getChessFields();
        m_chessBoardBut[takenPiece.first][takenPiece.second]->setIcon(
            m_imagesOfPieces[chessFields[takenPiece.first][takenPiece.second]]);
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

void ChessBoard::clickField(QString nameField)
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

    if (takenPiece.first != SIDE_SIZE) {
        if (std::any_of(beatFields.begin(), beatFields.end(), [ = ](const auto & p) {
        return p.first == i && p.second == j;
    })) {
            if (!m_transformPawn && chessFields[takenPiece.first][takenPiece.second][1] == 'P' && (i == 0 || i == 7)) {
                if (m_autoQueen) {
                    m_game.setField(color + 'Q', takenPiece.first, takenPiece.second);
                } else {
                    m_transformPawn = true;
                    m_game.chooseTransformPawn(j);
                    this->transformPawnField(beatFields);
                    this->updateBoardIcon();
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

            this->uncheckLastMove();
            this->uncheckKing();

            m_game.movePiece(std::pair{i, j});

            const auto lastMove = m_game.getLastMove();
            m_chessBoard->moveField(lastMove.second.first, lastMove.second.second);
            this->checkKing();

            if (!m_game.isPossibleMove()) {
                if (m_game.getCheck())
                    emit endGame(whiteMove ? ResultGame::WIN_WHITE : ResultGame::WIN_BLACK);
                else
                    emit endGame(ResultGame::STALE_MATE);
            } else if (m_game.isStaleMate()) {
                emit endGame(ResultGame::STALE_MATE);
            }

            this->untakePiece();
            this->updateBoardIcon();
            if (!m_blockBoard)
                emit didMove();
        } else {
            if (m_transformPawn) {
                m_transformPawn = false;
                m_game.untransformPawn();
                this->untransformPawnField(beatFields);
            }

            if (chessFields[takenPiece.first][takenPiece.second][1] == 'K' && m_game.getCheck())
                this->checkKing();
            else
                m_chessBoard->baseField(takenPiece.first, takenPiece.second);

            if ((takenPiece.first != i || takenPiece.second != j)
                    && !chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
                this->takePiece(i, j);
            } else {
                this->untakePiece();
                m_game.untakePiece();
            }
        }
    } else if (!chessFields[i][j].isEmpty() && chessFields[i][j][0] == color) {
        this->takePiece(i, j);
    }

    m_whiteMove = m_game.getColorMove();
}

void ChessBoard::uncheckLastMove()
{
    const auto lastMove = m_game.getLastMove();
    if (lastMove.first.first != SIDE_SIZE)
        m_chessBoard->baseField(lastMove.first.first, lastMove.first.second);
    if (lastMove.second.first != SIDE_SIZE)
        m_chessBoard->baseField(lastMove.second.first, lastMove.second.second);
}

void ChessBoard::checkKing()
{
    bool whiteMove = m_game.getColorMove();
    const auto posKings = m_game.getPosKings();
    if (m_game.getCheck()) {
        if (whiteMove)
            m_chessBoard->checkField(posKings.first.first, posKings.first.second);
        else
            m_chessBoard->checkField(posKings.second.first, posKings.second.second);
    }
}

void ChessBoard::uncheckKing()
{
    bool whiteMove = m_game.getColorMove();
    const auto posKings = m_game.getPosKings();
    if (m_game.getCheck()) {
        if (whiteMove)
            m_chessBoard->baseField(posKings.first.first, posKings.first.second);
        else
            m_chessBoard->baseField(posKings.second.first, posKings.second.second);
    }
}

void ChessBoard::takePiece(qint8 i, qint8 j)
{
    this->untakePiece();
    m_chessBoard->moveField(i, j);

    m_game.takePiece(i, j);
    const auto &chessFields = m_game.getChessFields();
    const auto &beatFields = m_game.getBeatFields();

    for (const auto &field : beatFields) {
        if (chessFields[field.first][field.second].isEmpty()) {
            m_chessBoard->setIconSize(field.first, field.second, BEAT_FIELD_SIZE);
            m_chessBoard->setIcon(field.first, field.second, "beatField");
        } else {
            m_chessBoard->setIcon(field.first, field.second, chessFields[field.first][field.second] + "beatPiece");
        }
    }
}

void ChessBoard::untakePiece()
{
    const auto &chessFields = m_game.getChessFields();
    const auto &beatFields = m_game.getBeatFields();

    for (const auto &field : beatFields)
        m_chessBoard->setIcon(field.first, field.second, chessFields[field.first][field.second]);

    m_game.untakePiece();
}

void ChessBoard::transformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        m_chessBoard->whiteField(field.first, field.second);
}

void ChessBoard::untransformPawnField(const std::vector<std::pair<qint8, qint8>> &beatFields)
{
    for (const auto &field : beatFields)
        m_chessBoard->baseField(field.first, field.second);
}

void ChessBoard::updateBoardIcon()
{
    const auto &chessFields = m_game.getChessFields();
    const auto &beatFields = m_game.getBeatFields();

    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        for (qint8 j = 0; j < SIDE_SIZE; ++j) {
            m_chessBoard->setIconSize(i, j);
            m_chessBoard->setIcon(i, j, chessFields[i][j]);
        }
    }

    for (const auto &field : beatFields) {
        if (chessFields[field.first][field.second].isEmpty()) {
            m_chessBoard->setIconSize(field.first, field.second, BEAT_FIELD_SIZE);
            m_chessBoard->setIcon(field.first, field.second, "beatField");
        } else {
            m_chessBoard->setIcon(field.first, field.second, chessFields[field.first][field.second] + "beatPiece");
        }
    }
}

void ChessBoard::updateBoardSize()
{
    const auto &chessFields = m_game.getChessFields();
    const auto &beatFields = m_game.getBeatFields();

    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->setIconSize(i, j);

    for (const auto &field : beatFields)
        if (chessFields[field.first][field.second].isEmpty())
            m_chessBoard->setIconSize(field.first, field.second, BEAT_FIELD_SIZE);
}

void ChessBoard::updateHistoryScene()
{
    this->uncheckLastMove();
    this->uncheckKing();
    m_game.historyMove();
    const auto lastMove = m_game.getLastMove();
    if (lastMove.first.first != SIDE_SIZE)
        m_chessBoard->moveField(lastMove.first.first, lastMove.first.second);
    if (lastMove.second.first != SIDE_SIZE)
        m_chessBoard->moveField(lastMove.second.first, lastMove.second.second);
    this->checkKing();
    this->updateBoardIcon();
}
