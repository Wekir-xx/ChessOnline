#include "chess_game.h"

#include <algorithm>
#include <cstdlib>

#include <QChar>

ChessGame::ChessGame()
    : m_whiteInfo{true},
      m_blackInfo{false},
      m_whiteMove{true},
      m_check{false},
      m_takenPiece{SIDE_SIZE, SIDE_SIZE},
      m_numBoardHistory{0},
      m_movesHistory{0}
{
    m_beatFields.reserve(30);
    m_chessMoveHistory.reserve(1000);
    m_chessFieldsHistory.reserve(1000);
    m_savePiece.reserve(4);
}

void ChessGame::movePiece(std::pair<int, int> newPos)
{
    QString endMove;

    if (m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second][1] == 'K') {
        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second].clear();

        if (m_whiteMove)
            endMove = this->castlingKing(newPos, m_whiteInfo, m_boardParams.castling.first);
        else
            endMove = this->castlingKing(newPos, m_blackInfo, m_boardParams.castling.second);
    } else {
        if (m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second][1] == 'R') {
            if (m_whiteMove)
                this->moveRook(m_whiteInfo, m_boardParams.castling.first);
            else
                this->moveRook(m_blackInfo, m_boardParams.castling.second);
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

    m_takenPiece = {SIDE_SIZE, SIDE_SIZE};
}

void ChessGame::takePiece(int i, int j)
{
    m_takenPiece = {i, j};
    m_beatFields.clear();
    const auto namePiece = m_boardParams.chessFields[i][j].toStdString();

    switch (namePiece[1]) {
    case 'K':
        for (int row = i - 1; row < i + 2; ++row)
            for (int col = j - 1; col < j + 2; ++col)
                if ((row != i || col != j) && this->checkMove(row, col, true))
                    m_beatFields.push_back({row, col});

        this->addCastling();
        break;

    case 'Q':
        this->addMoves(RookMoves);
        this->addMoves(BishopMoves);
        break;

    case 'R':
        this->addMoves(RookMoves);
        break;

    case 'B':
        this->addMoves(BishopMoves);
        break;

    case 'N':
        for (const auto &[rangeRow, rangeCol] : KnightMoves) {
            const int row = i + rangeRow;
            const int col = j + rangeCol;
            if (this->checkMove(row, col))
                m_beatFields.push_back({row, col});
        }
        break;

    case 'P':
    {
        const int row = m_whiteMove ? 1 : -1;

        if (m_boardParams.chessFields[i + row][j].isEmpty() && this->checkMove(i + row, j))
            m_beatFields.push_back({i + row, j});
        if (this->checkMove(i + row, j + 1)
                && !m_boardParams.chessFields[i + row][j + 1].isEmpty())
            m_beatFields.push_back({i + row, j + 1});
        if (this->checkMove(i + row, j - 1)
                && !m_boardParams.chessFields[i + row][j - 1].isEmpty())
            m_beatFields.push_back({i + row, j - 1});

        if (m_whiteMove) {
            if (i == 1 && m_boardParams.chessFields[2][j].isEmpty()
                && m_boardParams.chessFields[3][j].isEmpty() && this->checkMove(3, j))
                m_beatFields.push_back({3, j});

            if (i == 4) {
                if (m_lastMove.first  == std::pair<int, int> {6, j + 1} && m_lastMove.second == std::pair<int, int> {4, j + 1}
                    && m_boardParams.chessFields[4][j + 1] == "bP" && this->checkEnPassant(5, j + 1, 4, j + 1))
                    m_beatFields.push_back({5, j + 1});

                if (m_lastMove.first  == std::pair<int, int> {6, j - 1} && m_lastMove.second == std::pair<int, int> {4, j - 1}
                    && m_boardParams.chessFields[4][j - 1] == "bP" && this->checkEnPassant(5, j - 1, 4, j - 1))
                    m_beatFields.push_back({5, j - 1});
            }
        } else {
            if (i == 6 && m_boardParams.chessFields[4][j].isEmpty()
                && m_boardParams.chessFields[5][j].isEmpty() && this->checkMove(4, j))
                m_beatFields.push_back({4, j});

            if (i == 3) {
                if (m_lastMove.first  == std::pair<int, int> {1, j + 1} && m_lastMove.second == std::pair<int, int> {3, j + 1}
                    && m_boardParams.chessFields[3][j + 1] == "wP" && this->checkEnPassant(2, j + 1, 3, j + 1))
                    m_beatFields.push_back({2, j + 1});

                if (m_lastMove.first  == std::pair<int, int> {1, j - 1} && m_lastMove.second == std::pair<int, int> {3, j - 1}
                    && m_boardParams.chessFields[3][j - 1] == "wP" && this->checkEnPassant(2, j - 1, 3, j - 1))
                    m_beatFields.push_back({2, j - 1});
            }
        }
        break;
    }
    }
}

void ChessGame::untakePiece()
{
    m_takenPiece = {SIDE_SIZE, SIDE_SIZE};
    m_beatFields.clear();
}

bool ChessGame::isPossibleMove()
{
    const auto savedTakenPiece = m_takenPiece;
    const auto savedBeat = m_beatFields;

    bool flag = this->isPossibleMoveInner();

    m_beatFields = savedBeat;
    m_takenPiece = savedTakenPiece;
    return flag;
}

bool ChessGame::isStaleMate()
{
    if (m_movesHistory >= 100)
        return true;

    if (m_movesHistory > 2) {
        auto beginIt = m_chessFieldsHistory.crbegin();
        int num = 0;
        for (int i = m_movesHistory; i > 0; --i, ++beginIt) {
            if (*beginIt == m_boardParams.chessFields) {
                ++num;
                if (num == 3)
                    return true;
            }
        }
    }

    std::vector<QString> piece;
    std::vector<std::pair<int, int>> coord;
    for (int i = 0; i < SIDE_SIZE; ++i) {
        for (int j = 0; j < SIDE_SIZE; ++j) {
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

    if (piece.size() > 1 && !(((piece[0] == "wB" && piece[1] == "bB") || (piece[0] == "bB" && piece[1] == "wB"))
                              && (coord[0].first + coord[0].second) % 2 == (coord[1].first + coord[1].second) % 2))
        return false;

    return true;
}

bool ChessGame::isCheck()
{
    int i, j;
    PieceInfo *info;

    if (m_whiteMove) {
        i = m_whiteInfo.posKing.first;
        j = m_whiteInfo.posKing.second;
        info = &m_blackInfo.info;
    } else {
        i = m_blackInfo.posKing.first;
        j = m_blackInfo.posKing.second;
        info = &m_whiteInfo.info;
    }

    const auto &board = m_boardParams.chessFields;

    auto inBounds = [](int row, int col) {
        return row >= 0 && row < SIDE_SIZE && col >= 0 && col < SIDE_SIZE;
    };

    for (int rangeRow = -1; rangeRow <= 1; ++rangeRow)
        for (int rangeCol = -1; rangeCol <= 1; ++rangeCol) {
            if (rangeRow == 0 && rangeCol == 0)
                continue;

            const int row = i + rangeRow;
            const int col = j + rangeCol;
            if (inBounds(row, col) && board[row][col] == info->king)
                return true;
        }

    const int pawnRow = m_whiteMove ? i + 1 : i - 1;
    for (int rangeCol : {-1, 1}) {
        const int col = j + rangeCol;
        if (inBounds(pawnRow, col) && board[pawnRow][col] == info->pawn)
            return true;
    }

    for (const auto &[rangeRow, rangeCol] : RookMoves) {
        for (int row = i + rangeRow, col = j + rangeCol; inBounds(row, col); row += rangeRow, col += rangeCol) {
            if (board[row][col].isEmpty())
                continue;
            if (board[row][col][0] == info->color && (board[row][col][1] == 'R' || board[row][col][1] == 'Q'))
                return true;
            break;
        }
    }

    for (const auto &[rangeRow, rangeCol] : BishopMoves) {
        for (int row = i + rangeRow, col = j + rangeCol; inBounds(row, col); row += rangeRow, col += rangeCol) {
            if (board[row][col].isEmpty())
                continue;
            if (board[row][col][0] == info->color && (board[row][col][1] == 'B' || board[row][col][1] == 'Q'))
                return true;
            break;
        }
    }

    for (const auto &[rangeRow, rangeCol] : KnightMoves) {
        const int row = i + rangeRow;
        const int col = j + rangeCol;
        if (inBounds(row, col) && board[row][col] == info->knight)
            return true;
    }

    return false;
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
    m_lastMove = std::pair{std::pair{static_cast<int>(lastMove[0].digitValue()),
                                     static_cast<int>(lastMove[1].digitValue())},
                           std::pair{static_cast<int>(lastMove[2].digitValue()),
                                     static_cast<int>(lastMove[3].digitValue())}};

    for (int i = 0; i < SIDE_SIZE; ++i) {
        for (int j = 0; j < SIDE_SIZE; ++j) {
            if (m_boardParams.chessFields[i][j] == "wK")
                m_whiteInfo.posKing = {i, j};
            else if (m_boardParams.chessFields[i][j] == "bK")
                m_blackInfo.posKing = {i, j};
        }
    }

    m_whiteMove ^= true;
    m_check = this->isCheck();
}

void ChessGame::chooseTransformPawn(int j)
{
    m_beatFields.clear();

    if (m_whiteMove) {
        for (int row = SIDE_SIZE / 2; row < SIDE_SIZE; ++row) {
            m_savePiece.push_back(m_boardParams.chessFields[row][j]);
            m_beatFields.push_back({row, j});
        }

        m_boardParams.chessFields[7][j] = "wQ";
        m_boardParams.chessFields[6][j] = "wR";
        m_boardParams.chessFields[5][j] = "wB";
        m_boardParams.chessFields[4][j] = "wN";
    } else {
        for (int row = 0; row < SIDE_SIZE / 2; ++row) {
            m_savePiece.push_back(m_boardParams.chessFields[row][j]);
            m_beatFields.push_back({row, j});
        }

        m_boardParams.chessFields[0][j] = "bQ";
        m_boardParams.chessFields[1][j] = "bR";
        m_boardParams.chessFields[2][j] = "bB";
        m_boardParams.chessFields[3][j] = "bN";
    }
}

void ChessGame::transformPawn(int i, int j)
{
    QString choosePiece = m_boardParams.chessFields[i][j];
    this->untransformPawn();
    m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second] = choosePiece;
}

void ChessGame::untransformPawn()
{
    for (int num = 0; num < SIDE_SIZE / 2; ++num)
        m_boardParams.chessFields[m_beatFields[num].first][m_beatFields[num].second] = m_savePiece[num];

    m_savePiece.clear();
}

void ChessGame::setField(QString field, int i, int j)
{
    m_boardParams.chessFields[i][j] = field;
}

void ChessGame::setColorMove(bool whiteMove)
{
    m_whiteMove = whiteMove;
}

void ChessGame::setCheck(bool flag)
{
    m_check = flag;
}

void ChessGame::setChessParams(ChessBoardParams &boardParams)
{
    m_boardParams = boardParams;
    m_whiteMove = boardParams.whiteMove;

    m_whiteInfo.posRooks = {{SIDE_SIZE, SIDE_SIZE}, {SIDE_SIZE, SIDE_SIZE}};
    m_blackInfo.posRooks = {{SIDE_SIZE, SIDE_SIZE}, {SIDE_SIZE, SIDE_SIZE}};

    for (int i = 0; i < SIDE_SIZE; ++i) {
        for (int j = 0; j < SIDE_SIZE; ++j) {
            if (boardParams.chessFields[i][j] == "wK") {
                m_whiteInfo.posKing = {i, j};
            } else if (boardParams.chessFields[i][j] == "bK") {
                m_blackInfo.posKing = {i, j};
            } else if (boardParams.chessFields[i][j] == "wR") {
                if (m_whiteInfo.posRooks.first.first == SIDE_SIZE)
                    m_whiteInfo.posRooks.first = {i, j};
                else
                    m_whiteInfo.posRooks.second = {i, j};
            } else if (boardParams.chessFields[i][j] == "bR") {
                if (m_blackInfo.posRooks.first.first == SIDE_SIZE)
                    m_blackInfo.posRooks.first = {i, j};
                else
                    m_blackInfo.posRooks.second = {i, j};
            }
        }
    }

    m_beatFields.clear();
    m_chessMoveHistory.clear();
    m_chessFieldsHistory.clear();
    m_savePiece.clear();
    m_movesHistory = 1;
    m_numBoardHistory = 0;
    m_takenPiece = {SIDE_SIZE, SIDE_SIZE};
    m_lastMove = std::pair{m_takenPiece, m_takenPiece};

    QString str = QString::number(SIDE_SIZE);
    m_chessMoveHistory.push_back(str + str + str + str);
    m_chessFieldsHistory.push_back(m_boardParams.chessFields);

    m_check = this->isCheck();
}

bool ChessGame::getCheck()
{
    return m_check;
}

bool ChessGame::getColorMove()
{
    return m_whiteMove;
}

std::pair<int, int> ChessGame::getTakenPiece()
{
    return m_takenPiece;
}

const std::vector<std::vector<QString>> &ChessGame::getChessFields()
{
    return m_boardParams.chessFields;
}

const std::vector<std::pair<int, int>> &ChessGame::getBeatFields()
{
    return m_beatFields;
}

std::pair<std::pair<int, int>, std::pair<int, int>> ChessGame::getLastMove()
{
    return m_lastMove;
}

std::pair<std::pair<int, int>, std::pair<int, int>> ChessGame::getPosKings()
{
    return {m_whiteInfo.posKing, m_blackInfo.posKing};
}

QString ChessGame::castlingKing(std::pair<int, int> newPos, baseInfo& info, std::pair<bool, bool>& castling)
{
    QString endMove;
    if (castling.first && (newPos.second == 2 || newPos.second == info.posRooks.first.second)) {
        m_boardParams.chessFields[info.info.lastLine][info.posRooks.first.second].clear();
        m_boardParams.chessFields[info.info.lastLine][2] = info.info.king;
        m_boardParams.chessFields[info.info.lastLine][3] = info.info.rook;
        info.posKing = {info.info.lastLine, 2};
        m_lastMove = std::pair{m_takenPiece, info.posKing};
        endMove = "00";
    } else if (castling.second && (newPos.second == 6 || newPos.second == info.posRooks.second.second)) {
        m_boardParams.chessFields[info.info.lastLine][info.posRooks.second.second].clear();
        m_boardParams.chessFields[info.info.lastLine][6] = info.info.king;
        m_boardParams.chessFields[info.info.lastLine][5] = info.info.rook;
        info.posKing = {info.info.lastLine, 6};
        m_lastMove = std::pair{m_takenPiece, info.posKing};
        endMove = "0";
    } else {
        m_boardParams.chessFields[newPos.first][newPos.second] = info.info.king;
        info.posKing = {newPos.first, newPos.second};
        m_lastMove = std::pair{m_takenPiece, newPos};
    }

    castling = {false, false};
    return endMove;
}

void ChessGame::moveRook(const baseInfo &info, std::pair<bool, bool>& castling)
{
    if (m_takenPiece.first == info.posRooks.first.first && m_takenPiece.second == info.posRooks.first.second)
        castling.first = false;
    else if (m_takenPiece.first == info.posRooks.second.first && m_takenPiece.second == info.posRooks.second.second)
        castling.second = false;
}

bool ChessGame::checkMove(int i, int j, bool isKing)
{
    if (i >= 0 && i < SIDE_SIZE && j >= 0 && j < SIDE_SIZE
            && (m_boardParams.chessFields[i][j].isEmpty() || (m_whiteMove && m_boardParams.chessFields[i][j][0] == 'b')
                || (!m_whiteMove && m_boardParams.chessFields[i][j][0] == 'w'))) {
        QString deletePiece = m_boardParams.chessFields[i][j];
        m_boardParams.chessFields[i][j] = m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second];
        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second].clear();

        if (isKing) {
            if (m_whiteMove)
                m_whiteInfo.posKing = {i, j};
            else
                m_blackInfo.posKing = {i, j};
        }

        bool possibleMove = !this->isCheck();

        m_boardParams.chessFields[m_takenPiece.first][m_takenPiece.second] = m_boardParams.chessFields[i][j];
        m_boardParams.chessFields[i][j] = deletePiece;

        if (isKing) {
            if (m_whiteMove)
                m_whiteInfo.posKing = {m_takenPiece.first, m_takenPiece.second};
            else
                m_blackInfo.posKing = {m_takenPiece.first, m_takenPiece.second};
        }

        return possibleMove;
    }
    return false;
}

bool ChessGame::checkEnPassant(int toRow, int toCol, int capRow, int capCol)
{
    const int fromRow = m_takenPiece.first;
    const int fromCol = m_takenPiece.second;

    const QString savedTo  = m_boardParams.chessFields[toRow][toCol];
    const QString savedCap = m_boardParams.chessFields[capRow][capCol];

    m_boardParams.chessFields[toRow][toCol] = m_boardParams.chessFields[fromRow][fromCol];
    m_boardParams.chessFields[fromRow][fromCol].clear();
    m_boardParams.chessFields[capRow][capCol].clear();

    const bool possible = !this->isCheck();

    m_boardParams.chessFields[fromRow][fromCol] = m_boardParams.chessFields[toRow][toCol];
    m_boardParams.chessFields[toRow][toCol]   = savedTo;
    m_boardParams.chessFields[capRow][capCol] = savedCap;

    return possible;
}

bool ChessGame::isPossibleMoveInner()
{
    QChar color;
    if (m_whiteMove)
        color = 'w';
    else
        color = 'b';

    for (int row = 0; row < SIDE_SIZE; ++row) {
        for (int col = 0; col < SIDE_SIZE; ++col) {
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

    auto posKingCol = m_whiteInfo.posKing.second;
    auto posRooks = m_whiteInfo.posRooks;
    auto castling = m_boardParams.castling.first;

    if (!m_whiteMove) {
        posKingCol = m_blackInfo.posKing.second;
        posRooks = m_blackInfo.posRooks;
        castling = m_boardParams.castling.second;
    }

    int row = posRooks.first.first;

    if (m_boardParams.chess960) {
        auto innerTryCastling = [&](int rookCol, int kingFinalCol, int rookFinalCol) -> bool {
            const int leftCol = std::min({posKingCol, rookCol, kingFinalCol, rookFinalCol});
            const int rightCol = std::max({posKingCol, rookCol, kingFinalCol, rookFinalCol});
            for (int col = leftCol; col <= rightCol; ++col) {
                if (col == posKingCol || col == rookCol)
                    continue;
                if (!m_boardParams.chessFields[row][col].isEmpty())
                    return false;
            }

            const int kingLeftCol = std::min(posKingCol, kingFinalCol);
            const int kingRightCol = std::max(posKingCol, kingFinalCol);
            for (int col = kingLeftCol; col <= kingRightCol; ++col) {
                if (col == posKingCol)
                    continue;
                if (!this->checkMove(row, col, true))
                    return false;
            }

            return true;
        };

        auto tryCastling = [&](int rookCol, int kingFinalCol, int rookFinalCol) {
            const QString rook = m_boardParams.chessFields[row][rookCol];
            m_boardParams.chessFields[row][rookCol].clear();

            if (innerTryCastling(rookCol, kingFinalCol, rookFinalCol)) {
                m_beatFields.push_back({row, rookCol});
                if (rookCol != kingFinalCol && std::abs(posKingCol - kingFinalCol) > 1)
                    m_beatFields.push_back({row, kingFinalCol});
            }

            m_boardParams.chessFields[row][rookCol] = rook;
        };

        if (castling.first)
            tryCastling(posRooks.first.second, 2, 3);
        if (castling.second)
            tryCastling(posRooks.second.second, 6, 5);
    } else {
        if (castling.first && m_boardParams.chessFields[row][1].isEmpty()
                && m_boardParams.chessFields[row][2].isEmpty() && this->checkMove(row, 2, true)
                && m_boardParams.chessFields[row][3].isEmpty() && this->checkMove(row, 3, true))
            m_beatFields.push_back({row, 2});

        if (castling.second && m_boardParams.chessFields[row][5].isEmpty() && this->checkMove(row, 5, true)
                            && m_boardParams.chessFields[row][6].isEmpty() && this->checkMove(row, 6, true))
            m_beatFields.push_back({row, 6});
    }
}

void ChessGame::addMoves(const std::pair<int, int> *moves)
{
    for (int i = 0; i < 4; ++i) {
        const int rangeRow = moves[i].first;
        const int rangeCol = moves[i].second;

        for (int row = m_takenPiece.first + rangeRow, col = m_takenPiece.second + rangeCol;
             row >= 0 && row < SIDE_SIZE && col >= 0 && col < SIDE_SIZE; row += rangeRow, col += rangeCol) {

            if (this->checkMove(row, col))
                m_beatFields.push_back({row, col});

            if (!m_boardParams.chessFields[row][col].isEmpty())
                break;
        }
    }
}
