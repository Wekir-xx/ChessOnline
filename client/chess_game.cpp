#include "chess_game.h"

ChessGame::ChessGame()
{
    m_beatField.reserve(30);
}

void ChessGame::movePiece(std::pair<qint8, qint8> oldPos, std::pair<qint8, qint8> newPos)
{
    if (m_chess.chessBoard[oldPos.first][oldPos.second][1] == 'K') {
        m_chess.chessBoard[oldPos.first][oldPos.second].clear();

        if (m_whiteMove) {
            if (m_castling.first.first
                && (newPos.second == 2 || newPos.second == m_chess.posRooksWhite.first.second)) {
                m_chess.chessBoard[0][m_chess.posRooksWhite.first.second].clear();
                m_chess.chessBoard[0][2] = "wK";
                m_chess.chessBoard[0][3] = "wR";
                m_chess.posKings.first = {0, 2};
                m_lastMove = std::pair{oldPos, m_chess.posKings.first};
            } else if (m_castling.first.second
                       && (newPos.second == 6
                           || newPos.second == m_chess.posRooksWhite.second.second)) {
                m_chess.chessBoard[0][m_chess.posRooksWhite.second.second].clear();
                m_chess.chessBoard[0][6] = "wK";
                m_chess.chessBoard[0][5] = "wR";
                m_chess.posKings.first = {0, 6};
                m_lastMove = std::pair{oldPos, m_chess.posKings.first};
            } else {
                m_chess.chessBoard[newPos.first][newPos.second] = "wK";
                m_chess.posKings.first = {newPos.first, newPos.second};
                m_lastMove = std::pair{oldPos, newPos};
            }
            m_castling.first = {false, false};
        } else {
            if (m_castling.second.first
                && (newPos.second == 2 || newPos.second == m_chess.posRooksBlack.first.second)) {
                m_chess.chessBoard[7][m_chess.posRooksBlack.first.second].clear();
                m_chess.chessBoard[7][2] = "bK";
                m_chess.chessBoard[7][3] = "bR";
                m_chess.posKings.second = {7, 2};
                m_lastMove = std::pair{oldPos, m_chess.posKings.second};
            } else if (m_castling.second.second
                       && (newPos.second == 6
                           || newPos.second == m_chess.posRooksBlack.second.second)) {
                m_chess.chessBoard[7][m_chess.posRooksBlack.second.second].clear();
                m_chess.chessBoard[7][6] = "bK";
                m_chess.chessBoard[7][5] = "bR";
                m_chess.posKings.second = {7, 6};
                m_lastMove = std::pair{oldPos, m_chess.posKings.second};
            } else {
                m_chess.chessBoard[newPos.first][newPos.second] = "bK";
                m_chess.posKings.second = {newPos.first, newPos.second};
                m_lastMove = std::pair{oldPos, newPos};
            }
            m_castling.second = {false, false};
        }
    } else {
        if (m_chess.chessBoard[oldPos.first][oldPos.second][1] == 'R') {
            if (m_whiteMove) {
                if (oldPos.first == m_chess.posRooksWhite.first.first
                    && oldPos.second == m_chess.posRooksWhite.first.second)
                    m_castling.first.first = false;
                else if (oldPos.first == m_chess.posRooksWhite.second.first
                         && oldPos.second == m_chess.posRooksWhite.second.second)
                    m_castling.first.second = false;
            } else {
                if (oldPos.first == m_chess.posRooksBlack.first.first
                    && oldPos.second == m_chess.posRooksBlack.first.second)
                    m_castling.second.first = false;
                else if (oldPos.first == m_chess.posRooksBlack.second.first
                         && oldPos.second == m_chess.posRooksBlack.second.second)
                    m_castling.second.second = false;
            }
        } else if (m_chess.chessBoard[oldPos.first][oldPos.second][1] == 'P'
                   && m_chess.chessBoard[newPos.first][newPos.second].isEmpty()
                   && oldPos.second != newPos.second) {
            m_chess.chessBoard[oldPos.first][newPos.second].clear();
        }

        m_chess.chessBoard[newPos.first][newPos.second]
            = m_chess.chessBoard[oldPos.first][oldPos.second];
        m_chess.chessBoard[oldPos.first][oldPos.second].clear();
        m_lastMove = std::pair{oldPos, newPos};
    }

    m_whiteMove ^= true;
}

std::vector<std::pair<qint8, qint8>> &ChessGame::takePiece(qint8 i, qint8 j)
{
    m_takenPiece = {i, j};
    m_beatField.clear();
    const auto namePiece = m_chess.chessBoard[i][j].toStdString();

    switch (namePiece[1]) {
    case 'K':
        for (qint8 row = i - 1; row < i + 2; ++row)
            for (qint8 col = j - 1; col < j + 2; ++col)
                if ((row != i || col != j) && checkMove(row, col, true))
                    m_beatField.push_back({row, col});

        if ((!(m_whiteMove && (m_castling.first.first || m_castling.first.second))
             && !(!m_whiteMove && (m_castling.second.first || m_castling.second.second)))
            || isCheck())
            break;

        addCastling();
        break;

    case 'Q':
        addMovesRook();
        addMovesBishop();
        break;

    case 'R':
        addMovesRook();
        break;

    case 'B':
        addMovesBishop();
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

    case 'P':
        if (m_whiteMove) {
            if (m_chess.chessBoard[i + 1][j].isEmpty() && checkMove(i + 1, j))
                m_beatField.push_back({i + 1, j});
            if (i == 1 && m_chess.chessBoard[2][j].isEmpty() && m_chess.chessBoard[3][j].isEmpty()
                && checkMove(3, j))
                m_beatField.push_back({3, j});
            if (checkMove(i + 1, j + 1) && !m_chess.chessBoard[i + 1][j + 1].isEmpty())
                m_beatField.push_back({i + 1, j + 1});
            if (checkMove(i + 1, j - 1) && !m_chess.chessBoard[i + 1][j - 1].isEmpty())
                m_beatField.push_back({i + 1, j - 1});
            if (i == 4) {
                if (checkMove(4, j + 1) && m_chess.chessBoard[4][j + 1] == "bP"
                    && m_lastMove.first == std::pair<qint8, qint8>{6, j + 1}
                    && m_lastMove.second == std::pair<qint8, qint8>{4, j + 1})
                    m_beatField.push_back({5, j + 1});
                if (checkMove(4, j - 1) && m_chess.chessBoard[4][j - 1] == "bP"
                    && m_lastMove.first == std::pair<qint8, qint8>{6, j - 1}
                    && m_lastMove.second == std::pair<qint8, qint8>{4, j - 1})
                    m_beatField.push_back({5, j - 1});
            }
        } else {
            if (m_chess.chessBoard[i - 1][j].isEmpty() && checkMove(i - 1, j))
                m_beatField.push_back({i - 1, j});
            if (i == 6 && m_chess.chessBoard[4][j].isEmpty() && m_chess.chessBoard[5][j].isEmpty()
                && checkMove(4, j))
                m_beatField.push_back({4, j});
            if (checkMove(i - 1, j + 1) && !m_chess.chessBoard[i - 1][j + 1].isEmpty())
                m_beatField.push_back({i - 1, j + 1});
            if (checkMove(i - 1, j - 1) && !m_chess.chessBoard[i - 1][j - 1].isEmpty())
                m_beatField.push_back({i - 1, j - 1});
            if (i == 3) {
                if (checkMove(3, j + 1) && m_chess.chessBoard[3][j + 1] == "wP"
                    && m_lastMove.first == std::pair<qint8, qint8>{1, j + 1}
                    && m_lastMove.second == std::pair<qint8, qint8>{3, j + 1})
                    m_beatField.push_back({2, j + 1});
                if (checkMove(3, j - 1) && m_chess.chessBoard[3][j - 1] == "wP"
                    && m_lastMove.first == std::pair<qint8, qint8>{1, j - 1}
                    && m_lastMove.second == std::pair<qint8, qint8>{3, j - 1})
                    m_beatField.push_back({2, j - 1});
            }
        }
        break;
    }

    return m_beatField;
}

bool ChessGame::isCheck()
{
    qint8 i, j;

    if (m_whiteMove) {
        i = m_chess.posKings.first.first;
        j = m_chess.posKings.first.second;
    } else {
        i = m_chess.posKings.second.first;
        j = m_chess.posKings.second.second;
    }

    if (((i + 1) < 8 && !m_chess.chessBoard[i + 1][j].isEmpty()
         && m_chess.chessBoard[i + 1][j][1] == 'K')
        || ((i - 1) >= 0 && !m_chess.chessBoard[i - 1][j].isEmpty()
            && m_chess.chessBoard[i - 1][j][1] == 'K')
        || ((j + 1) < 8 && !m_chess.chessBoard[i][j + 1].isEmpty()
            && m_chess.chessBoard[i][j + 1][1] == 'K')
        || ((j - 1) >= 0 && !m_chess.chessBoard[i][j - 1].isEmpty()
            && m_chess.chessBoard[i][j - 1][1] == 'K')
        || ((i + 1) < 8 && (j + 1) < 8 && !m_chess.chessBoard[i + 1][j + 1].isEmpty()
            && m_chess.chessBoard[i + 1][j + 1][1] == 'K')
        || ((i + 1) < 8 && (j - 1) >= 0 && !m_chess.chessBoard[i + 1][j - 1].isEmpty()
            && m_chess.chessBoard[i + 1][j - 1][1] == 'K')
        || ((i - 1) >= 0 && (j + 1) < 8 && !m_chess.chessBoard[i - 1][j + 1].isEmpty()
            && m_chess.chessBoard[i - 1][j + 1][1] == 'K')
        || ((i - 1) >= 0 && (j - 1) >= 0 && !m_chess.chessBoard[i - 1][j - 1].isEmpty()
            && m_chess.chessBoard[i - 1][j - 1][1] == 'K'))
        return true;

    if (m_whiteMove) {
        if (((i + 1) < 8 && (j + 1) < 8 && m_chess.chessBoard[i + 1][j + 1] == "bP")
            || ((i + 1) < 8 && (j - 1) >= 0 && m_chess.chessBoard[i + 1][j - 1] == "bP"))
            return true;

        for (qint8 col = j + 1; col < 8; ++col) {
            if (!m_chess.chessBoard[i][col].isEmpty()) {
                if (m_chess.chessBoard[i][col][0] == 'b'
                    && (m_chess.chessBoard[i][col][1] == 'R'
                        || m_chess.chessBoard[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 col = j - 1; col >= 0; --col) {
            if (!m_chess.chessBoard[i][col].isEmpty()) {
                if (m_chess.chessBoard[i][col][0] == 'b'
                    && (m_chess.chessBoard[i][col][1] == 'R'
                        || m_chess.chessBoard[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1; row < 8; ++row) {
            if (!m_chess.chessBoard[row][j].isEmpty()) {
                if (m_chess.chessBoard[row][j][0] == 'b'
                    && (m_chess.chessBoard[row][j][1] == 'R'
                        || m_chess.chessBoard[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1; row >= 0; --row) {
            if (!m_chess.chessBoard[row][j].isEmpty()) {
                if (m_chess.chessBoard[row][j][0] == 'b'
                    && (m_chess.chessBoard[row][j][1] == 'R'
                        || m_chess.chessBoard[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j + 1; row < 8 && col < 8; ++row, ++col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'b'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j - 1; row >= 0 && col >= 0; --row, --col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'b'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j + 1; row >= 0 && col < 8; --row, ++col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'b'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j - 1; row < 8 && col >= 0; ++row, --col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'b'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < 8 && (j + 1) < 8 && m_chess.chessBoard[i + 2][j + 1] == "bN")
            || ((i + 2) < 8 && (j - 1) >= 0 && m_chess.chessBoard[i + 2][j - 1] == "bN")
            || ((i - 2) >= 0 && (j + 1) < 8 && m_chess.chessBoard[i - 2][j + 1] == "bN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_chess.chessBoard[i - 2][j - 1] == "bN")
            || ((i + 1) < 8 && (j + 2) < 8 && m_chess.chessBoard[i + 1][j + 2] == "bN")
            || ((i + 1) < 8 && (j - 2) >= 0 && m_chess.chessBoard[i + 1][j - 2] == "bN")
            || ((i - 1) >= 0 && (j + 2) < 8 && m_chess.chessBoard[i - 1][j + 2] == "bN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_chess.chessBoard[i - 1][j - 2] == "bN"))
            return true;

    } else {
        if (((i - 1) >= 0 && (j + 1) < 8 && m_chess.chessBoard[i - 1][j + 1] == "wP")
            || ((i - 1) >= 0 && (j - 1) >= 0 && m_chess.chessBoard[i - 1][j - 1] == "wP"))
            return true;

        for (qint8 col = j + 1; col < 8; ++col) {
            if (!m_chess.chessBoard[i][col].isEmpty()) {
                if (m_chess.chessBoard[i][col][0] == 'w'
                    && (m_chess.chessBoard[i][col][1] == 'R'
                        || m_chess.chessBoard[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 col = j - 1; col >= 0; --col) {
            if (!m_chess.chessBoard[i][col].isEmpty()) {
                if (m_chess.chessBoard[i][col][0] == 'w'
                    && (m_chess.chessBoard[i][col][1] == 'R'
                        || m_chess.chessBoard[i][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1; row < 8; ++row) {
            if (!m_chess.chessBoard[row][j].isEmpty()) {
                if (m_chess.chessBoard[row][j][0] == 'w'
                    && (m_chess.chessBoard[row][j][1] == 'R'
                        || m_chess.chessBoard[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1; row >= 0; --row) {
            if (!m_chess.chessBoard[row][j].isEmpty()) {
                if (m_chess.chessBoard[row][j][0] == 'w'
                    && (m_chess.chessBoard[row][j][1] == 'R'
                        || m_chess.chessBoard[row][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j + 1; row < 8 && col < 8; ++row, ++col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'w'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j - 1; row >= 0 && col >= 0; --row, --col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'w'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i - 1, col = j + 1; row >= 0 && col < 8; --row, ++col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'w'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (qint8 row = i + 1, col = j - 1; row < 8 && col >= 0; ++row, --col) {
            if (!m_chess.chessBoard[row][col].isEmpty()) {
                if (m_chess.chessBoard[row][col][0] == 'w'
                    && (m_chess.chessBoard[row][col][1] == 'B'
                        || m_chess.chessBoard[row][col][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < 8 && (j + 1) < 8 && m_chess.chessBoard[i + 2][j + 1] == "wN")
            || ((i + 2) < 8 && (j - 1) >= 0 && m_chess.chessBoard[i + 2][j - 1] == "wN")
            || ((i - 2) >= 0 && (j + 1) < 8 && m_chess.chessBoard[i - 2][j + 1] == "wN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_chess.chessBoard[i - 2][j - 1] == "wN")
            || ((i + 1) < 8 && (j + 2) < 8 && m_chess.chessBoard[i + 1][j + 2] == "wN")
            || ((i + 1) < 8 && (j - 2) >= 0 && m_chess.chessBoard[i + 1][j - 2] == "wN")
            || ((i - 1) >= 0 && (j + 2) < 8 && m_chess.chessBoard[i - 1][j + 2] == "wN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_chess.chessBoard[i - 1][j - 2] == "wN"))
            return true;
    }

    return false;
}

bool ChessGame::isPossibleMove()
{
    QChar color;

    if (m_whiteMove)
        color = 'w';
    else
        color = 'b';

    for (qint8 row = 0; row < 8; ++row)
        for (qint8 col = 0; col < 8; ++col)
            if (!m_chess.chessBoard[row][col].isEmpty() && m_chess.chessBoard[row][col][0] == color)
                if (!takePiece(row, col).empty())
                    return true;

    return false;
}

void ChessGame::setChessParams(ChessParams chess)
{
    m_chess = chess;
    m_whiteMove = true;
    m_castling = {{true, true}, {true, true}};

    if (m_chess.posRooksWhite.first.first != 0)
        m_castling.first.first = false;
    if (m_chess.posRooksWhite.second.first != 0)
        m_castling.first.second = false;
    if (m_chess.posRooksBlack.first.first != 7)
        m_castling.second.first = false;
    if (m_chess.posRooksBlack.second.first != 7)
        m_castling.second.second = false;
}

std::vector<std::vector<QString>> &ChessGame::getChessBoard()
{
    return m_chess.chessBoard;
}

std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> &ChessGame::getLastMove()
{
    return m_lastMove;
}

std::pair<std::pair<qint8, qint8>, std::pair<qint8, qint8>> &ChessGame::getPosKings()
{
    return m_chess.posKings;
}

bool ChessGame::checkMove(qint8 i, qint8 j, bool isKing)
{
    if (i >= 0 && i < 8 && j >= 0 && j < 8
        && (m_chess.chessBoard[i][j].isEmpty() || (m_whiteMove && m_chess.chessBoard[i][j][0] == 'b')
            || (!m_whiteMove && m_chess.chessBoard[i][j][0] == 'w'))) {
        QString deletePiece = m_chess.chessBoard[i][j];
        m_chess.chessBoard[i][j] = m_chess.chessBoard[m_takenPiece.first][m_takenPiece.second];
        m_chess.chessBoard[m_takenPiece.first][m_takenPiece.second].clear();

        if (isKing) {
            if (m_whiteMove)
                m_chess.posKings.first = {i, j};
            else
                m_chess.posKings.second = {i, j};
        }

        bool possibleMove = !isCheck();

        m_chess.chessBoard[m_takenPiece.first][m_takenPiece.second] = m_chess.chessBoard[i][j];
        m_chess.chessBoard[i][j] = deletePiece;

        if (isKing) {
            if (m_whiteMove)
                m_chess.posKings.first = {m_takenPiece.first, m_takenPiece.second};
            else
                m_chess.posKings.second = {m_takenPiece.first, m_takenPiece.second};
        }

        return possibleMove;
    }
    return false;
}

void ChessGame::addCastling()
{
    auto posKingCol = m_chess.posKings.first.second;
    auto posRooks = m_chess.posRooksWhite;
    auto castling = m_castling.first;

    if (!m_whiteMove) {
        posKingCol = m_chess.posKings.second.second;
        posRooks = m_chess.posRooksBlack;
        castling = m_castling.second;
    }

    qint8 row = posRooks.first.first;

    if (m_chess.chess960) {
        auto posRookCol = posRooks.first.second;
        QString rook = m_chess.chessBoard[row][posRookCol];
        bool possibleCastling = true;

        if (castling.first) {
            m_chess.chessBoard[row][posRookCol].clear();

            for (qint8 col = (posKingCol - 1) <= 3 ? 3 : (posKingCol - 1); col > 1; --col) {
                if (col != posKingCol
                    && (!m_chess.chessBoard[row][col].isEmpty() || !checkMove(row, col, true))) {
                    possibleCastling = false;
                    break;
                }
            }

            if (possibleCastling) {
                m_beatField.push_back({row, posRookCol});

                if (posRookCol != 2 && posKingCol > 3)
                    m_beatField.push_back({row, 2});
            } else {
                possibleCastling = true;
            }

            m_chess.chessBoard[row][posRookCol] = rook;
        }

        if (castling.second) {
            posRookCol = posRooks.second.second;
            m_chess.chessBoard[row][posRookCol].clear();

            for (qint8 col = (posKingCol + 1) >= 5 ? 5 : (posKingCol + 1); col < 7; ++col) {
                if (col != posKingCol
                    && (!m_chess.chessBoard[row][col].isEmpty() || !checkMove(row, col, true))) {
                    possibleCastling = false;
                    break;
                }
            }

            if (possibleCastling) {
                m_beatField.push_back({row, posRookCol});

                if (posRookCol != 6 && posKingCol < 5)
                    m_beatField.push_back({row, 6});
            }

            m_chess.chessBoard[row][posRookCol] = rook;
        }
    } else {
        if (castling.first && m_chess.chessBoard[row][2].isEmpty() && checkMove(row, 2, true)
            && m_chess.chessBoard[row][3].isEmpty() && checkMove(row, 3, true))
            m_beatField.push_back({row, 2});

        if (castling.second && m_chess.chessBoard[row][5].isEmpty() && checkMove(row, 5, true)
            && m_chess.chessBoard[row][6].isEmpty() && checkMove(row, 6, true))
            m_beatField.push_back({row, 6});
    }
}

void ChessGame::addMovesRook()
{
    for (qint8 col = m_takenPiece.second + 1; col < 8; ++col) {
        if (checkMove(m_takenPiece.first, col))
            m_beatField.push_back({m_takenPiece.first, col});

        if (!m_chess.chessBoard[m_takenPiece.first][col].isEmpty())
            break;
    }
    for (qint8 col = m_takenPiece.second - 1; col >= 0; --col) {
        if (checkMove(m_takenPiece.first, col))
            m_beatField.push_back({m_takenPiece.first, col});

        if (!m_chess.chessBoard[m_takenPiece.first][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first + 1; row < 8; ++row) {
        if (checkMove(row, m_takenPiece.second))
            m_beatField.push_back({row, m_takenPiece.second});

        if (!m_chess.chessBoard[row][m_takenPiece.second].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1; row >= 0; --row) {
        if (checkMove(row, m_takenPiece.second))
            m_beatField.push_back({row, m_takenPiece.second});

        if (!m_chess.chessBoard[row][m_takenPiece.second].isEmpty())
            break;
    }
}

void ChessGame::addMovesBishop()
{
    for (qint8 row = m_takenPiece.first + 1, col = m_takenPiece.second + 1; row < 8 && col < 8;
         ++row, ++col) {
        if (checkMove(row, col))
            m_beatField.push_back({row, col});

        if (!m_chess.chessBoard[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1, col = m_takenPiece.second - 1; row >= 0 && col >= 0;
         --row, --col) {
        if (checkMove(row, col))
            m_beatField.push_back({row, col});

        if (!m_chess.chessBoard[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first - 1, col = m_takenPiece.second + 1; row >= 0 && col < 8;
         --row, ++col) {
        if (checkMove(row, col))
            m_beatField.push_back({row, col});

        if (!m_chess.chessBoard[row][col].isEmpty())
            break;
    }
    for (qint8 row = m_takenPiece.first + 1, col = m_takenPiece.second - 1; row < 8 && col >= 0;
         ++row, --col) {
        if (checkMove(row, col))
            m_beatField.push_back({row, col});

        if (!m_chess.chessBoard[row][col].isEmpty())
            break;
    }
}
