#include "chess_game.h"

ChessGame::ChessGame()
{
    m_beatField.reserve(100);
}

void ChessGame::movePiece(std::pair<int, int> oldPos, std::pair<int, int> newPos)
{
    m_chessBoard[newPos.first][newPos.second] = m_chessBoard[oldPos.first][oldPos.second];
    m_chessBoard[oldPos.first][oldPos.second].clear();
    m_whiteMove ^= true;
}

std::vector<std::pair<int, int>> &ChessGame::takePiece(
    int i, int j, std::pair<std::pair<int, int>, std::pair<int, int>> m_lastMove)
{
    m_takenPiece = {i, j};
    m_beatField.clear();
    const auto namePiece = m_chessBoard[i][j].toStdString();

    switch (namePiece[1]) {
    case 'K':
        for (int row = i - 1; row < i + 2; ++row)
            for (int col = j - 1; col < j + 2; ++col)
                if ((row != i || col != j) && checkMove(row, col, true))
                    m_beatField.push_back({row, col});
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
        if (namePiece[0] == 'w') {
            if (m_chessBoard[i + 1][j].isEmpty() && checkMove(i + 1, j))
                m_beatField.push_back({i + 1, j});
            if (i == 1 && m_chessBoard[2][j].isEmpty() && m_chessBoard[3][j].isEmpty()
                && checkMove(3, j))
                m_beatField.push_back({3, j});
            if (checkMove(i + 1, j + 1) && !m_chessBoard[i + 1][j + 1].isEmpty())
                m_beatField.push_back({i + 1, j + 1});
            if (checkMove(i + 1, j - 1) && !m_chessBoard[i + 1][j - 1].isEmpty())
                m_beatField.push_back({i + 1, j - 1});
            if (i == 4) {
                if (checkMove(4, j + 1) && m_chessBoard[4][j + 1] == "bP"
                    && m_lastMove.first == std::pair{4, j + 1}
                    && m_lastMove.second == std::pair{6, j + 1})
                    m_beatField.push_back({5, j + 1});
                if (checkMove(4, j - 1) && m_chessBoard[4][j - 1] == "bP"
                    && m_lastMove.first == std::pair{4, j - 1}
                    && m_lastMove.second == std::pair{6, j - 1})
                    m_beatField.push_back({5, j - 1});
            }
        } else {
            if (m_chessBoard[i - 1][j].isEmpty() && checkMove(i - 1, j))
                m_beatField.push_back({i - 1, j});
            if (i == 6 && m_chessBoard[4][j].isEmpty() && m_chessBoard[5][j].isEmpty()
                && checkMove(4, j))
                m_beatField.push_back({4, j});
            if (checkMove(i - 1, j + 1) && !m_chessBoard[i - 1][j + 1].isEmpty())
                m_beatField.push_back({i - 1, j + 1});
            if (checkMove(i - 1, j - 1) && !m_chessBoard[i - 1][j - 1].isEmpty())
                m_beatField.push_back({i - 1, j - 1});
            if (i == 3) {
                if (checkMove(3, j + 1) && m_chessBoard[3][j + 1] == "wP"
                    && m_lastMove.first == std::pair{3, j + 1}
                    && m_lastMove.second == std::pair{1, j + 1})
                    m_beatField.push_back({2, j + 1});
                if (checkMove(3, j - 1) && m_chessBoard[3][j - 1] == "wP"
                    && m_lastMove.first == std::pair{3, j - 1}
                    && m_lastMove.second == std::pair{1, j - 1})
                    m_beatField.push_back({2, j - 1});
            }
        }
        break;
    }

    return m_beatField;
}

bool ChessGame::isCheck()
{
    int i, j;

    if (m_whiteMove) {
        i = m_posKings.first.first;
        j = m_posKings.first.second;
    } else {
        i = m_posKings.second.first;
        j = m_posKings.second.second;
        qDebug() << m_posKings.second;
    }

    if (((i + 1) < 8 && !m_chessBoard[i + 1][j].isEmpty() && m_chessBoard[i + 1][j][1] == 'K')
        || ((i - 1) >= 0 && !m_chessBoard[i - 1][j].isEmpty() && m_chessBoard[i - 1][j][1] == 'K')
        || ((j + 1) < 8 && !m_chessBoard[i][j + 1].isEmpty() && m_chessBoard[i][j + 1][1] == 'K')
        || ((j - 1) >= 0 && !m_chessBoard[i][j - 1].isEmpty() && m_chessBoard[i][j - 1][1] == 'K')
        || ((i + 1) < 8 && !m_chessBoard[i + 1][j + 1].isEmpty() && (j + 1) < 8
            && m_chessBoard[i + 1][j + 1][1] == 'K')
        || ((i + 1) < 8 && !m_chessBoard[i + 1][j - 1].isEmpty() && (j - 1) >= 0
            && m_chessBoard[i + 1][j - 1][1] == 'K')
        || ((i - 1) >= 0 && !m_chessBoard[i - 1][j + 1].isEmpty() && (j + 1) < 8
            && m_chessBoard[i - 1][j + 1][1] == 'K')
        || ((i - 1) >= 0 && !m_chessBoard[i - 1][j - 1].isEmpty() && (j - 1) >= 0
            && m_chessBoard[i - 1][j - 1][1] == 'K'))
        return true;

    if (m_whiteMove) {
        if (((i + 1) < 8 && (j + 1) < 8 && m_chessBoard[i + 1][j + 1] == "bP")
            || ((i + 1) < 8 && (j - 1) >= 0 && m_chessBoard[i + 1][j - 1] == "bP"))
            return true;

        for (int j2 = j + 1; j2 < 8; ++j2) {
            if (!m_chessBoard[i][j2].isEmpty()) {
                if (m_chessBoard[i][j2][0] == 'b'
                    && (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int j2 = j - 1; j2 >= 0; --j2) {
            if (!m_chessBoard[i][j2].isEmpty()) {
                if (m_chessBoard[i][j2][0] == 'b'
                    && (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1; i2 < 8; ++i2) {
            if (!m_chessBoard[i2][j].isEmpty()) {
                if (m_chessBoard[i2][j][0] == 'b'
                    && (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1; i2 >= 0; --i2) {
            if (!m_chessBoard[i2][j].isEmpty()) {
                if (m_chessBoard[i2][j][0] == 'b'
                    && (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8; ++i2, ++j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'b'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1, j2 = j - 1; i2 >= 0 && j2 >= 0; --i2, --j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'b'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1, j2 = j + 1; i2 >= 0 && j2 < 8; --i2, ++j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'b'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1, j2 = j - 1; i2 < 8 && j2 >= 0; ++i2, --j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'b'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < 8 && (j + 1) < 8 && m_chessBoard[i + 2][j + 1] == "bN")
            || ((i + 2) < 8 && (j - 1) >= 0 && m_chessBoard[i + 2][j - 1] == "bN")
            || ((i - 2) >= 0 && (j + 1) < 8 && m_chessBoard[i - 2][j + 1] == "bN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_chessBoard[i - 2][j - 1] == "bN")
            || ((i + 1) < 8 && (j + 2) < 8 && m_chessBoard[i + 1][j + 2] == "bN")
            || ((i - 1) >= 0 && (j + 2) < 8 && m_chessBoard[i - 1][j + 2] == "bN")
            || ((i + 1) < 8 && (j - 2) >= 0 && m_chessBoard[i + 1][j + 2] == "bN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_chessBoard[i - 1][j + 2] == "bN"))
            return true;

    } else {
        if (((i - 1) >= 0 && (j + 1) < 8 && m_chessBoard[i - 1][j + 1] == "wP")
            || ((i - 1) >= 0 && (j - 1) >= 0 && m_chessBoard[i - 1][j - 1] == "wP"))
            return true;

        for (int j2 = j + 1; j2 < 8; ++j2) {
            if (!m_chessBoard[i][j2].isEmpty()) {
                if (m_chessBoard[i][j2][0] == 'w'
                    && (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int j2 = j - 1; j2 >= 0; --j2) {
            if (!m_chessBoard[i][j2].isEmpty()) {
                if (m_chessBoard[i][j2][0] == 'w'
                    && (m_chessBoard[i][j2][1] == 'R' || m_chessBoard[i][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1; i2 < 8; ++i2) {
            if (!m_chessBoard[i2][j].isEmpty()) {
                if (m_chessBoard[i2][j][0] == 'w'
                    && (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1; i2 >= 0; --i2) {
            if (!m_chessBoard[i2][j].isEmpty()) {
                if (m_chessBoard[i2][j][0] == 'w'
                    && (m_chessBoard[i2][j][1] == 'R' || m_chessBoard[i2][j][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1, j2 = j + 1; i2 < 8 && j2 < 8; ++i2, ++j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'w'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1, j2 = j - 1; i2 >= 0 && j2 >= 0; --i2, --j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'w'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i - 1, j2 = j + 1; i2 >= 0 && j2 < 8; --i2, ++j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'w'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        for (int i2 = i + 1, j2 = j - 1; i2 < 8 && j2 >= 0; ++i2, --j2) {
            if (!m_chessBoard[i2][j2].isEmpty()) {
                if (m_chessBoard[i2][j2][0] == 'w'
                    && (m_chessBoard[i2][j2][1] == 'B' || m_chessBoard[i2][j2][1] == 'Q')) {
                    return true;
                }
                break;
            }
        }

        if (((i + 2) < 8 && (j + 1) < 8 && m_chessBoard[i + 2][j + 1] == "wN")
            || ((i + 2) < 8 && (j - 1) >= 0 && m_chessBoard[i + 2][j - 1] == "wN")
            || ((i - 2) >= 0 && (j + 1) < 8 && m_chessBoard[i - 2][j + 1] == "wN")
            || ((i - 2) >= 0 && (j - 1) >= 0 && m_chessBoard[i - 2][j - 1] == "wN")
            || ((i + 1) < 8 && (j + 2) < 8 && m_chessBoard[i + 1][j + 2] == "wN")
            || ((i - 1) >= 0 && (j + 2) < 8 && m_chessBoard[i - 1][j + 2] == "wN")
            || ((i + 1) < 8 && (j - 2) >= 0 && m_chessBoard[i + 1][j + 2] == "wN")
            || ((i - 1) >= 0 && (j - 2) >= 0 && m_chessBoard[i - 1][j + 2] == "wN"))
            return true;
    }

    return false;
}

bool ChessGame::isMate()
{
    return false;
}

void ChessGame::setChessBoard(std::vector<std::vector<QString>> chessBoard)
{
    m_chessBoard = chessBoard;
}

void ChessGame::setPosKings(std::pair<std::pair<int, int>, std::pair<int, int>> posKings)
{
    m_posKings = posKings;
}

std::vector<std::vector<QString>> &ChessGame::getChessBoard()
{
    return m_chessBoard;
}

bool ChessGame::checkMove(int i, int j, bool isKing)
{
    if (i >= 0 && i < 8 && j >= 0 && j < 8
        && (m_chessBoard[i][j].isEmpty() || (m_whiteMove && m_chessBoard[i][j][0] == 'b')
            || (!m_whiteMove && m_chessBoard[i][j][0] == 'w'))) {
        QString deletePiece = m_chessBoard[i][j];
        m_chessBoard[i][j] = m_chessBoard[m_takenPiece.first][m_takenPiece.second];
        m_chessBoard[m_takenPiece.first][m_takenPiece.second].clear();

        if (isKing) {
            if (m_whiteMove)
                m_posKings.first = {i, j};
            else
                m_posKings.second = {i, j};
        }

        bool possibleMove = !isCheck();

        m_chessBoard[m_takenPiece.first][m_takenPiece.second] = m_chessBoard[i][j];
        m_chessBoard[i][j] = deletePiece;

        if (isKing) {
            if (m_whiteMove)
                m_posKings.first = {m_takenPiece.first, m_takenPiece.second};
            else
                m_posKings.second = {m_takenPiece.first, m_takenPiece.second};
        }

        return possibleMove;
    }
    return false;
}

void ChessGame::addMovesRook()
{
    for (int col = m_takenPiece.second + 1; col < 8; ++col) {
        if (checkMove(m_takenPiece.first, col)) {
            m_beatField.push_back({m_takenPiece.first, col});
            if (!m_chessBoard[m_takenPiece.first][col].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int col = m_takenPiece.second - 1; col >= 0; --col) {
        if (checkMove(m_takenPiece.first, col)) {
            m_beatField.push_back({m_takenPiece.first, col});
            if (!m_chessBoard[m_takenPiece.first][col].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int row = m_takenPiece.first + 1; row < 8; ++row) {
        if (checkMove(row, m_takenPiece.second)) {
            m_beatField.push_back({row, m_takenPiece.second});
            if (!m_chessBoard[row][m_takenPiece.second].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int row = m_takenPiece.first - 1; row >= 0; --row) {
        if (checkMove(row, m_takenPiece.second)) {
            m_beatField.push_back({row, m_takenPiece.second});
            if (!m_chessBoard[row][m_takenPiece.second].isEmpty())
                break;
        } else {
            break;
        }
    }
}

void ChessGame::addMovesBishop()
{
    for (int row = m_takenPiece.first + 1, col = m_takenPiece.second + 1; row < 8 && col < 8;
         ++row, ++col) {
        if (checkMove(row, col)) {
            m_beatField.push_back({row, col});
            if (!m_chessBoard[row][col].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int row = m_takenPiece.first - 1, col = m_takenPiece.second - 1; row >= 0 && col >= 0;
         --row, --col) {
        if (checkMove(row, col)) {
            m_beatField.push_back({row, col});
            if (!m_chessBoard[row][col].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int row = m_takenPiece.first - 1, col = m_takenPiece.second + 1; row >= 0 && col < 8;
         --row, ++col) {
        if (checkMove(row, col)) {
            m_beatField.push_back({row, col});
            if (!m_chessBoard[row][col].isEmpty())
                break;
        } else {
            break;
        }
    }
    for (int row = m_takenPiece.first + 1, col = m_takenPiece.second - 1; row < 8 && col >= 0;
         ++row, --col) {
        if (checkMove(row, col)) {
            m_beatField.push_back({row, col});
            if (!m_chessBoard[row][col].isEmpty())
                break;
        } else {
            break;
        }
    }
}
