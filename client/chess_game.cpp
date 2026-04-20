#include "chess_game.h"

#include <QChar>

ChessGame::ChessGame()
{
    m_beatFields.reserve(30);
    m_chessMoveHistory.reserve(1000);
    m_chessFieldsHistory.reserve(1000);
    m_savePiece.reserve(4);
}

void ChessGame::movePiece(std::pair<qint8, qint8> newPos)
{
    QString endMove;
    if (m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second][1] == 'K') {
        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second].clear();

        if (m_whiteMove) {
            if (m_boardParams.castling.first.first && (newPos.second == 2 || newPos.second == posRooksWhite.first.second)) {
                m_boardParams.chessFields[0][posRooksWhite.first.second].clear();
                m_boardParams.chessFields[0][2] = "wK";
                m_boardParams.chessFields[0][3] = "wR";
                posKings.first = {0, 2};
                m_lastMove = std::pair{m_takenPiece, posKings.first};
                endMove = "00";
            } else if (m_boardParams.castling.first.second && (newPos.second == 6 || newPos.second == posRooksWhite.second.second)) {
                m_boardParams.chessFields[0][posRooksWhite.second.second].clear();
                m_boardParams.chessFields[0][6] = "wK";
                m_boardParams.chessFields[0][5] = "wR";
                posKings.first = {0, 6};
                m_lastMove = std::pair{m_takenPiece, posKings.first};
                endMove = "0";
            } else {
                m_boardParams.chessFields[newPos.first][newPos.second] = "wK";
                posKings.first = {newPos.first, newPos.second};
                m_lastMove = std::pair{m_takenPiece, newPos};
            }
            m_boardParams.castling.first = {false, false};
        } else {
            if (m_boardParams.castling.second.first && (newPos.second == 2 || newPos.second == posRooksBlack.first.second)) {
                m_boardParams.chessFields[7][posRooksBlack.first.second].clear();
                m_boardParams.chessFields[7][2] = "bK";
                m_boardParams.chessFields[7][3] = "bR";
                posKings.second = {7, 2};
                m_lastMove = std::pair{m_takenPiece, posKings.second};
                endMove = "00";
            } else if (m_boardParams.castling.second.second && (newPos.second == 6 || newPos.second == posRooksBlack.second.second)) {
                m_boardParams.chessFields[7][posRooksBlack.second.second].clear();
                m_boardParams.chessFields[7][6] = "bK";
                m_boardParams.chessFields[7][5] = "bR";
                posKings.second = {7, 6};
                m_lastMove = std::pair{m_takenPiece, posKings.second};
                endMove = "0";
            } else {
                m_boardParams.chessFields[newPos.first][newPos.second] = "bK";
                posKings.second = {newPos.first, newPos.second};
                m_lastMove = std::pair{m_takenPiece, newPos};
            }
            if (m_boardParams.castling.second.first || m_boardParams.castling.second.second)
                m_movesHistory = 0;
            m_boardParams.castling.second = {false, false};
        }
    } else {
        if (m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second][1] == 'R') {
            if (m_whiteMove) {
                if (m_takenPiece.first == posRooksWhite.first.first && m_takenPiece.second == posRooksWhite.first.second) {
                    if (m_boardParams.castling.first.first)
                        m_movesHistory = 0;
                    m_boardParams.castling.first.first = false;
                } else if (m_takenPiece.first == posRooksWhite.second.first && m_takenPiece.second == posRooksWhite.second.second) {
                    if (m_boardParams.castling.first.second)
                        m_movesHistory = 0;
                    m_boardParams.castling.first.second = false;
                }
            } else {
                if (m_takenPiece.first == posRooksBlack.first.first && m_takenPiece.second == posRooksBlack.first.second) {
                    if (m_boardParams.castling.second.first)
                        m_movesHistory = 0;
                    m_boardParams.castling.second.first = false;
                } else if (m_takenPiece.first == posRooksBlack.second.first && m_takenPiece.second == posRooksBlack.second.second) {
                    if (m_boardParams.castling.second.second)
                        m_movesHistory = 0;
                    m_boardParams.castling.second.second = false;
                }
            }
        } else if (m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second][1] == 'P') {
            m_movesHistory = 0;
            if (m_boardParams.chessFields[newPos.first][newPos.second].isEmpty() && m_takenPiece.second != newPos.second)
                m_boardParams.chessFields[m_takenPiece.first][newPos.second].clear();
        }

        m_boardParams.chessFields[newPos.first][newPos.second] = m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second];
        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second].clear();
        m_lastMove = std::pair{m_takenPiece, newPos};
    }

    m_whiteMove ^= true;
    m_check = this->isCheck();
    m_chessFieldsHistory.push_back(m_boardParams.chessFields);
    m_chessMoveHistory.push_back(QString::number(m_lastMove.first.first) + QString::number(m_lastMove.first.second) +
                                 QString::number(m_lastMove.second.first) + QString::number(m_lastMove.second.second) +
                                 endMove);
    ++m_numBoardHistory;
    ++m_movesHistory;

    m_takenPiece.first = SIDE_SIZE;
}

void ChessGame::takePiece(qint8 i, qint8 j)
{
    m_takenPiece = {i, j};
    m_beatFields.clear();
    const auto namePiece = m_boardParams.chessFields[i][j].toStdString();

    switch (namePiece[1]) {
    case 'K':
        for (qint8 row = i - 1; row < i + 2; ++row)
            for (qint8 col = j - 1; col < j + 2; ++col)
                if ((row != i || col != j) && this->checkMove(row, col, true))
                    m_beatFields.push_back({row, col});

        this->addCastling();
        break;

    case 'Q':
        this->addMovesRook();
        this->addMovesBishop();
        break;

    case 'R':
        this->addMovesRook();
        break;

    case 'B':
        this->addMovesBishop();
        break;

    case 'N':
        if (this->checkMove(i + 2, j + 1))
            m_beatFields.push_back({i + 2, j + 1});
        if (this->checkMove(i + 2, j - 1))
            m_beatFields.push_back({i + 2, j - 1});
        if (this->checkMove(i - 2, j + 1))
            m_beatFields.push_back({i - 2, j + 1});
        if (this->checkMove(i - 2, j - 1))
            m_beatFields.push_back({i - 2, j - 1});
        if (this->checkMove(i + 1, j + 2))
            m_beatFields.push_back({i + 1, j + 2});
        if (this->checkMove(i - 1, j + 2))
            m_beatFields.push_back({i - 1, j + 2});
        if (this->checkMove(i + 1, j - 2))
            m_beatFields.push_back({i + 1, j - 2});
        if (this->checkMove(i - 1, j - 2))
            m_beatFields.push_back({i - 1, j - 2});
        break;

    case 'P':
        if (m_whiteMove) {
            if (m_boardParams.chessFields[i + 1][j].isEmpty() && this->checkMove(i + 1, j))
                m_beatFields.push_back({i + 1, j});
            if (i == 1 && m_boardParams.chessFields[2][j].isEmpty() && m_boardParams.chessFields[3][j].isEmpty() && this->checkMove(3, j))
                m_beatFields.push_back({3, j});
            if (this->checkMove(i + 1, j + 1) && !m_boardParams.chessFields[i + 1][j + 1].isEmpty())
                m_beatFields.push_back({i + 1, j + 1});
            if (this->checkMove(i + 1, j - 1) && !m_boardParams.chessFields[i + 1][j - 1].isEmpty())
                m_beatFields.push_back({i + 1, j - 1});
            if (i == 4) {
                if (this->checkMove(4, j + 1) && m_boardParams.chessFields[4][j + 1] == "bP"
                    && m_lastMove.first == std::pair<qint8, qint8>{6, j + 1} && m_lastMove.second == std::pair<qint8, qint8>{4, j + 1})
                    m_beatFields.push_back({5, j + 1});
                if (this->checkMove(4, j - 1) && m_boardParams.chessFields[4][j - 1] == "bP"
                    && m_lastMove.first == std::pair<qint8, qint8>{6, j - 1} && m_lastMove.second == std::pair<qint8, qint8>{4, j - 1})
                    m_beatFields.push_back({5, j - 1});
            }
        } else {
            if (m_boardParams.chessFields[i - 1][j].isEmpty() && this->checkMove(i - 1, j))
                m_beatFields.push_back({i - 1, j});
            if (i == 6 && m_boardParams.chessFields[4][j].isEmpty() && m_boardParams.chessFields[5][j].isEmpty() && this->checkMove(4, j))
                m_beatFields.push_back({4, j});
            if (this->checkMove(i - 1, j + 1) && !m_boardParams.chessFields[i - 1][j + 1].isEmpty())
                m_beatFields.push_back({i - 1, j + 1});
            if (this->checkMove(i - 1, j - 1) && !m_boardParams.chessFields[i - 1][j - 1].isEmpty())
                m_beatFields.push_back({i - 1, j - 1});
            if (i == 3) {
                if (this->checkMove(3, j + 1) && m_boardParams.chessFields[3][j + 1] == "wP"
                    && m_lastMove.first == std::pair<qint8, qint8>{1, j + 1} && m_lastMove.second == std::pair<qint8, qint8>{3, j + 1})
                    m_beatFields.push_back({2, j + 1});
                if (this->checkMove(3, j - 1) && m_boardParams.chessFields[3][j - 1] == "wP"
                    && m_lastMove.first == std::pair<qint8, qint8>{1, j - 1} && m_lastMove.second == std::pair<qint8, qint8>{3, j - 1})
                    m_beatFields.push_back({2, j - 1});
            }
        }
        break;
    }
}

void ChessGame::untakePiece()
{
    m_takenPiece.first = SIDE_SIZE;
    m_beatFields.clear();
}

bool ChessGame::isPossibleMove()
{
    const auto takenPiece = m_takenPiece;
    bool flag = this->isPossibleMoveInner();
    m_takenPiece = takenPiece;
    return flag;
}

bool ChessGame::isStaleMate()
{
    if (m_movesHistory == 50)
        return true;

    if (m_movesHistory > 2) {
        auto beginIt = m_chessFieldsHistory.crbegin();
        qint8 num = 0;
        for (qint16 i = m_movesHistory; i > 0; --i, ++beginIt) {
            if (*beginIt == m_boardParams.chessFields) {
                ++num;
                if (num == 3)
                    return true;
            }
        }
    }

    std::vector<QString> piece;
    std::vector<std::pair<qint8, qint8>> coord;
    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        for (qint8 j = 0; j < SIDE_SIZE; ++j) {
            if (!m_boardParams.chessFields[i][j].isEmpty()) {
                if (m_boardParams.chessFields[i][j][1] == 'P' || m_boardParams.chessFields[i][j][1] == 'R'
                    || m_boardParams.chessFields[i][j][1] == 'Q') {
                    return false;
                } else if (m_boardParams.chessFields[i][j][1] != 'K') {
                    if (piece.size() > 2)
                        return false;
                    piece.push_back(m_boardParams.chessFields[i][j]);
                    coord.push_back({i, j});
                }
            }
        }
    }

    if (piece.size() > 1 && !(((piece[0] == "wB" && piece[1] == "bB") || (piece[0] == "wB" && piece[1] == "bB"))
                              && (coord[0].first + coord[0].first) % 2 == (coord[1].first + coord[1].first) % 2))
        return false;

    return true;
}

bool ChessGame::isPossibleHistoryBack()
{
    if (m_numBoardHistory == 0)
        return false;
    --m_numBoardHistory;
    return true;
}

bool ChessGame::isPossibleHistoryForward()
{
    if (m_numBoardHistory == m_chessFieldsHistory.size() - 1)
        return false;
    ++m_numBoardHistory;
    return true;
}

void ChessGame::historyMove()
{
    m_boardParams.chessFields = m_chessFieldsHistory[m_numBoardHistory];
    QString lastMove = m_chessMoveHistory[m_numBoardHistory];
    m_lastMove = std::pair{std::pair{static_cast<qint8>(lastMove[0].digitValue()),
                                     static_cast<qint8>(lastMove[1].digitValue())},
                           std::pair{static_cast<qint8>(lastMove[2].digitValue()),
                                     static_cast<qint8>(lastMove[3].digitValue())}};

    for (size_t i = 0; i < SIDE_SIZE; ++i) {
        for (size_t j = 0; j < SIDE_SIZE; ++j) {
            if (m_boardParams.chessFields[i][j] == "wK")
                posKings.first = {i, j};
            else if (m_boardParams.chessFields[i][j] == "bK")
                posKings.second = {i, j};
        }
    }

    m_whiteMove ^= true;
    m_check = this->isCheck();
}

void ChessGame::chooseTransformPawn(qint8 j)
{
    m_beatFields.clear();

    if (m_whiteMove) {
        for (qint8 row = SIDE_SIZE / 2; row < SIDE_SIZE; ++row) {
            m_savePiece.push_back(m_boardParams.chessFields[row][j]);
            m_beatFields.push_back({row, j});
        }

        m_boardParams.chessFields[7][j] = "wQ";
        m_boardParams.chessFields[6][j] = "wR";
        m_boardParams.chessFields[5][j] = "wB";
        m_boardParams.chessFields[4][j] = "wN";
    } else {
        for (qint8 row = 0; row < SIDE_SIZE / 2; ++row) {
            m_savePiece.push_back(m_boardParams.chessFields[row][j]);
            m_beatFields.push_back({row, j});
        }

        m_boardParams.chessFields[0][j] = "bQ";
        m_boardParams.chessFields[1][j] = "bR";
        m_boardParams.chessFields[2][j] = "bB";
        m_boardParams.chessFields[3][j] = "bN";
    }
}

void ChessGame::transformPawn(qint8 i, qint8 j)
{
    QString choosePiece = m_boardParams.chessFields[i][j];
    this->untransformPawn();
    m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second] = choosePiece;
}

void ChessGame::untransformPawn()
{
    for (qint8 num = 0; num < 4; ++num)
        m_boardParams.chessFields[m_beatFields[num].first][m_beatFields[num].second] = m_savePiece[num];

    m_savePiece.clear();
}

void ChessGame::setField(QString field, qint8 i, qint8 j)
{
    m_boardParams.chessFields[i][j] = field;
}

void ChessGame::setChessParams(ChessBoardParams &boardParams)
{
    m_boardParams = boardParams;
    m_whiteMove = boardParams.whiteMove;

    posRooksWhite = {{SIDE_SIZE, SIDE_SIZE}, {SIDE_SIZE, SIDE_SIZE}};
    posRooksBlack = {{SIDE_SIZE, SIDE_SIZE}, {SIDE_SIZE, SIDE_SIZE}};

    for (size_t i = 0; i < SIDE_SIZE; ++i) {
        for (size_t j = 0; j < SIDE_SIZE; ++j) {
            if (boardParams.chessFields[i][j] == "wK") {
                posKings.first = {i, j};
            } else if (boardParams.chessFields[i][j] == "bK") {
                posKings.second = {i, j};
            } else if (boardParams.chessFields[i][j] == "wR") {
                if (posRooksWhite.first.first == SIDE_SIZE)
                    posRooksWhite.first = {i, j};
                else
                    posRooksWhite.second = {i, j};
            } else if (boardParams.chessFields[i][j] == "bR") {
                if (posRooksBlack.first.first == SIDE_SIZE)
                    posRooksBlack.first = {i, j};
                else
                    posRooksBlack.second = {i, j};
            }
        }
    }

    m_beatFields.clear();
    m_chessMoveHistory.clear();
    m_chessFieldsHistory.clear();
    m_savePiece.clear();
    m_movesHistory = 1;
    m_numBoardHistory = 0;
    m_takenPiece.first = SIDE_SIZE;
    m_lastMove = std::pair{m_takenPiece, m_takenPiece};

    QString str = QString::number(SIDE_SIZE);
    m_chessMoveHistory.push_back(str + str + str + str);
    m_chessFieldsHistory.push_back(m_boardParams.chessFields);
}

bool ChessGame::getCheck()
{
    return m_check;
}

bool ChessGame::getColorMove()
{
    return m_whiteMove;
}

std::pair<qint8, qint8> ChessGame::getTakenPiece()
{
    return m_takenPiece;
}

const std::vector<std::vector<QString>> &ChessGame::getChessFields()
{
    return m_boardParams.chessFields;
}

const std::vector<std::pair<qint8, qint8>> &ChessGame::getBeatFields()
{
    return m_beatFields;
}

std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> ChessGame::getLastMove()
{
    return m_lastMove;
}

std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> ChessGame::getPosKings()
{
    return posKings;
}

bool ChessGame::isCheck()
{
    qint8 i, j;

    if (m_whiteMove) {
        i = posKings.first.first;
        j = posKings.first.second;
    } else {
        i = posKings.second.first;
        j = posKings.second.second;
    }

    if (((i + 1) < SIDE_SIZE && !m_boardParams.chessFields[i + 1][j].isEmpty() && m_boardParams.chessFields[i + 1][j][1] == 'K')
        || ((i - 1) >= 0 && !m_boardParams.chessFields[i - 1][j].isEmpty() && m_boardParams.chessFields[i - 1][j][1] == 'K')
        || ((j + 1) < SIDE_SIZE && !m_boardParams.chessFields[i][j + 1].isEmpty() && m_boardParams.chessFields[i][j + 1][1] == 'K')
        || ((j - 1) >= 0 && !m_boardParams.chessFields[i][j - 1].isEmpty() && m_boardParams.chessFields[i][j - 1][1] == 'K')
        || ((i + 1) < SIDE_SIZE && (j + 1) < SIDE_SIZE && !m_boardParams.chessFields[i + 1][j + 1].isEmpty()
            && m_boardParams.chessFields[i + 1][j + 1][1] == 'K')
        || ((i + 1) < SIDE_SIZE && (j - 1) >= 0 && !m_boardParams.chessFields[i + 1][j - 1].isEmpty()
            && m_boardParams.chessFields[i + 1][j - 1][1] == 'K')
        || ((i - 1) >= 0 && (j + 1) < SIDE_SIZE && !m_boardParams.chessFields[i - 1][j + 1].isEmpty()
            && m_boardParams.chessFields[i - 1][j + 1][1] == 'K')
        || ((i - 1) >= 0 && (j - 1) >= 0 && !m_boardParams.chessFields[i - 1][j - 1].isEmpty()
            && m_boardParams.chessFields[i - 1][j - 1][1] == 'K'))
        return true;

    if (m_whiteMove) {
        if (((i + 1) < SIDE_SIZE && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i + 1][j + 1] == "bP")
            || ((i + 1) < SIDE_SIZE && (j - 1) >= 0 && m_boardParams.chessFields[i + 1][j - 1] == "bP"))
            return true;

        for (qint8 col = j + 1; col < SIDE_SIZE; ++col) {
            if (!m_boardParams.chessFields[i][col].isEmpty()) {
                if (m_boardParams.chessFields[i][col][0] == 'b'
                    && (m_boardParams.chessFields[i][col][1] == 'R' || m_boardParams.chessFields[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 col = j - 1; col >= 0; --col) {
            if (!m_boardParams.chessFields[i][col].isEmpty()) {
                if (m_boardParams.chessFields[i][col][0] == 'b'
                    && (m_boardParams.chessFields[i][col][1] == 'R' || m_boardParams.chessFields[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1; row < SIDE_SIZE; ++row) {
            if (!m_boardParams.chessFields[row][j].isEmpty()) {
                if (m_boardParams.chessFields[row][j][0] == 'b'
                    && (m_boardParams.chessFields[row][j][1] == 'R' || m_boardParams.chessFields[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1; row >= 0; --row) {
            if (!m_boardParams.chessFields[row][j].isEmpty()) {
                if (m_boardParams.chessFields[row][j][0] == 'b'
                    && (m_boardParams.chessFields[row][j][1] == 'R' || m_boardParams.chessFields[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j + 1; row < SIDE_SIZE && col < SIDE_SIZE; ++row, ++col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'b'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j - 1; row >= 0 && col >= 0; --row, --col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'b'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j + 1; row >= 0 && col < SIDE_SIZE; --row, ++col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'b'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j - 1; row < SIDE_SIZE && col >= 0; ++row, --col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'b'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < SIDE_SIZE && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i + 2][j + 1] == "bN")
            || ((i + 2) < SIDE_SIZE && (j - 1) >= 0 && m_boardParams.chessFields[i + 2][j - 1] == "bN")
            || ((i - 2) >= 0 && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i - 2][j + 1] == "bN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_boardParams.chessFields[i - 2][j - 1] == "bN")
            || ((i + 1) < SIDE_SIZE && (j + 2) < SIDE_SIZE && m_boardParams.chessFields[i + 1][j + 2] == "bN")
            || ((i + 1) < SIDE_SIZE && (j - 2) >= 0 && m_boardParams.chessFields[i + 1][j - 2] == "bN")
            || ((i - 1) >= 0 && (j + 2) < SIDE_SIZE && m_boardParams.chessFields[i - 1][j + 2] == "bN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_boardParams.chessFields[i - 1][j - 2] == "bN"))
            return true;

    } else {
        if (((i - 1) >= 0 && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i - 1][j + 1] == "wP")
            || ((i - 1) >= 0 && (j - 1) >= 0 && m_boardParams.chessFields[i - 1][j - 1] == "wP"))
            return true;

        for (qint8 col = j + 1; col < SIDE_SIZE; ++col) {
            if (!m_boardParams.chessFields[i][col].isEmpty()) {
                if (m_boardParams.chessFields[i][col][0] == 'w'
                    && (m_boardParams.chessFields[i][col][1] == 'R' || m_boardParams.chessFields[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 col = j - 1; col >= 0; --col) {
            if (!m_boardParams.chessFields[i][col].isEmpty()) {
                if (m_boardParams.chessFields[i][col][0] == 'w'
                    && (m_boardParams.chessFields[i][col][1] == 'R' || m_boardParams.chessFields[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1; row < SIDE_SIZE; ++row) {
            if (!m_boardParams.chessFields[row][j].isEmpty()) {
                if (m_boardParams.chessFields[row][j][0] == 'w'
                    && (m_boardParams.chessFields[row][j][1] == 'R' || m_boardParams.chessFields[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1; row >= 0; --row) {
            if (!m_boardParams.chessFields[row][j].isEmpty()) {
                if (m_boardParams.chessFields[row][j][0] == 'w'
                    && (m_boardParams.chessFields[row][j][1] == 'R' || m_boardParams.chessFields[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j + 1; row < SIDE_SIZE && col < SIDE_SIZE; ++row, ++col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'w'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j - 1; row >= 0 && col >= 0; --row, --col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'w'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j + 1; row >= 0 && col < SIDE_SIZE; --row, ++col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'w'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j - 1; row < SIDE_SIZE && col >= 0; ++row, --col) {
            if (!m_boardParams.chessFields[row][col].isEmpty()) {
                if (m_boardParams.chessFields[row][col][0] == 'w'
                    && (m_boardParams.chessFields[row][col][1] == 'B' || m_boardParams.chessFields[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < SIDE_SIZE && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i + 2][j + 1] == "wN")
            || ((i + 2) < SIDE_SIZE && (j - 1) >= 0 && m_boardParams.chessFields[i + 2][j - 1] == "wN")
            || ((i - 2) >= 0 && (j + 1) < SIDE_SIZE && m_boardParams.chessFields[i - 2][j + 1] == "wN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_boardParams.chessFields[i - 2][j - 1] == "wN")
            || ((i + 1) < SIDE_SIZE && (j + 2) < SIDE_SIZE && m_boardParams.chessFields[i + 1][j + 2] == "wN")
            || ((i + 1) < SIDE_SIZE && (j - 2) >= 0 && m_boardParams.chessFields[i + 1][j - 2] == "wN")
            || ((i - 1) >= 0 && (j + 2) < SIDE_SIZE && m_boardParams.chessFields[i - 1][j + 2] == "wN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_boardParams.chessFields[i - 1][j - 2] == "wN"))
            return true;
    }

    return false;
}

bool ChessGame::checkMove(qint8 i, qint8 j, bool isKing)
{
    if (i >= 0 && i < SIDE_SIZE && j >= 0 && j < SIDE_SIZE
        && (m_boardParams.chessFields[i][j].isEmpty() || (m_whiteMove && m_boardParams.chessFields[i][j][0] == 'b')
            || (!m_whiteMove && m_boardParams.chessFields[i][j][0] == 'w'))) {
        QString deletePiece = m_boardParams.chessFields[i][j];
        m_boardParams.chessFields[i][j] = m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second];
        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second].clear();

        if (isKing) {
            if (m_whiteMove)
                posKings.first = {i, j};
            else
                posKings.second = {i, j};
        }

        bool possibleMove = !this->isCheck();

        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second] = m_boardParams.chessFields[i][j];
        m_boardParams.chessFields[i][j] = deletePiece;

        if (isKing) {
            if (m_whiteMove)
                posKings.first = {m_takenPiece.first, m_takenPiece.second};
            else
                posKings.second = {m_takenPiece.first, m_takenPiece.second};
        }

        return possibleMove;
    }
    return false;
}

bool ChessGame::isPossibleMoveInner()
{
    QChar color;

    if (m_whiteMove)
        color = 'w';
    else
        color = 'b';

    for (qint8 row = 0; row < SIDE_SIZE; ++row) {
        for (qint8 col = 0; col < SIDE_SIZE; ++col) {
            if (!m_boardParams.chessFields[row][col].isEmpty() && m_boardParams.chessFields[row][col][0] == color) {
                this->takePiece(row, col);
                if (!m_beatFields.empty())
                    return true;
            }
        }
    }

    return false;
}

void ChessGame::addCastling()
{
    if ((!(m_whiteMove && (m_boardParams.castling.first.first || m_boardParams.castling.first.second))
         && !(!m_whiteMove && (m_boardParams.castling.second.first || m_boardParams.castling.second.second)))
        || m_check)
        return;

    auto posKingCol = posKings.first.second;
    auto posRooks = posRooksWhite;
    auto castling = m_boardParams.castling.first;

    if (!m_whiteMove) {
        posKingCol = posKings.second.second;
        posRooks = posRooksBlack;
        castling = m_boardParams.castling.second;
    }

    qint8 row = posRooks.first.first;

    if (m_boardParams.chess960) {
        auto posRookCol = posRooks.first.second;
        QString rook = m_boardParams.chessFields[row][posRookCol];
        bool possibleCastling = true;

        if (castling.first) {
            m_boardParams.chessFields[row][posRookCol].clear();

            for (qint8 col = (posKingCol - 1) <= 3 ? 3 : (posKingCol - 1); col > 1; --col) {
                if (col != posKingCol && (!m_boardParams.chessFields[row][col].isEmpty() || !this->checkMove(row, col, true))) {
                    possibleCastling = false;
                    break;
                }
            }

            if (possibleCastling) {
                m_beatFields.push_back({row, posRookCol});

                if (posRookCol != 2 && posKingCol > 3)
                    m_beatFields.push_back({row, 2});
            } else {
                possibleCastling = true;
            }

            m_boardParams.chessFields[row][posRookCol] = rook;
        }

        if (castling.second) {
            posRookCol = posRooks.second.second;
            m_boardParams.chessFields[row][posRookCol].clear();

            for (qint8 col = (posKingCol + 1) >= 5 ? 5 : (posKingCol + 1); col < 7; ++col) {
                if (col != posKingCol && (!m_boardParams.chessFields[row][col].isEmpty() || !this->checkMove(row, col, true))) {
                    possibleCastling = false;
                    break;
                }
            }

            if (possibleCastling) {
                m_beatFields.push_back({row, posRookCol});

                if (posRookCol != 6 && posKingCol < 5)
                    m_beatFields.push_back({row, 6});
            }

            m_boardParams.chessFields[row][posRookCol] = rook;
        }
    } else {
        if (castling.first && m_boardParams.chessFields[row][1].isEmpty() && m_boardParams.chessFields[row][2].isEmpty()
            && this->checkMove(row, 2, true) && m_boardParams.chessFields[row][3].isEmpty() && this->checkMove(row, 3, true))
            m_beatFields.push_back({row, 2});

        if (castling.second && m_boardParams.chessFields[row][5].isEmpty() && this->checkMove(row, 5, true)
            && m_boardParams.chessFields[row][6].isEmpty() && this->checkMove(row, 6, true))
            m_beatFields.push_back({row, 6});
    }
}

void ChessGame::addMovesRook()
{
    for (qint8 col = m_takenPiece.second + 1; col < SIDE_SIZE; ++col) {
        if (this->checkMove(m_takenPiece.first, col))
            m_beatFields.push_back({m_takenPiece.first, col});

        if (!m_boardParams.chessFields[m_takenPiece.first][col].isEmpty())
            break;
    }
    for (qint8 col = m_takenPiece.second - 1; col >= 0; --col) {
        if (this->checkMove(m_takenPiece.first, col))
            m_beatFields.push_back({m_takenPiece.first, col});

        if (!m_boardParams.chessFields[m_takenPiece.first][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first + 1; row < SIDE_SIZE; ++row) {
        if (this->checkMove(row, m_takenPiece.second))
            m_beatFields.push_back({row, m_takenPiece.second});

        if (!m_boardParams.chessFields[row][m_takenPiece.second].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1; row >= 0; --row) {
        if (this->checkMove(row, m_takenPiece.second))
            m_beatFields.push_back({row, m_takenPiece.second});

        if (!m_boardParams.chessFields[row][m_takenPiece.second].isEmpty())
            break;
    }
}

void ChessGame::addMovesBishop()
{
    for (qint8 row = m_takenPiece.first + 1, col = m_takenPiece.second + 1; row < SIDE_SIZE && col < SIDE_SIZE; ++row, ++col) {
        if (this->checkMove(row, col))
            m_beatFields.push_back({row, col});

        if (!m_boardParams.chessFields[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1, col = m_takenPiece.second - 1; row >= 0 && col >= 0; --row, --col) {
        if (this->checkMove(row, col))
            m_beatFields.push_back({row, col});

        if (!m_boardParams.chessFields[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1, col = m_takenPiece.second + 1; row >= 0 && col < SIDE_SIZE; --row, ++col) {
        if (this->checkMove(row, col))
            m_beatFields.push_back({row, col});

        if (!m_boardParams.chessFields[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first + 1, col = m_takenPiece.second - 1; row < SIDE_SIZE && col >= 0; ++row, --col) {
        if (this->checkMove(row, col))
            m_beatFields.push_back({row, col});

        if (!m_boardParams.chessFields[row][col].isEmpty())
            break;
    }
}
