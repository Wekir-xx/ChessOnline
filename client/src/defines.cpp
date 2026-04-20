#include "defines.h"

#include <random>

SomeConstans &SomeConstans::getInstance()
{
    static SomeConstans instance;
    return instance;
}

void SomeConstans::swapWidgetVBox(QVBoxLayout *layout, QWidget *widget1, QWidget *widget2)
{
    qint8 id1 = layout->indexOf(widget1);
    qint8 id2 = layout->indexOf(widget2);

    layout->removeWidget(widget1);
    layout->removeWidget(widget2);

    if (id1 < id2) {
        layout->insertWidget(id1, widget2);
        layout->insertWidget(id2, widget1);
    } else {
        layout->insertWidget(id2, widget1);
        layout->insertWidget(id1, widget2);
    }
}

void SomeConstans::swapWidgetHBox(QHBoxLayout *layout, QWidget *widget1, QWidget *widget2)
{
    qint8 id1 = layout->indexOf(widget1);
    qint8 id2 = layout->indexOf(widget2);

    layout->removeWidget(widget1);
    layout->removeWidget(widget2);

    if (id1 < id2) {
        layout->insertWidget(id1, widget2);
        layout->insertWidget(id2, widget1);
    } else {
        layout->insertWidget(id2, widget1);
        layout->insertWidget(id1, widget2);
    }
}

void SomeConstans::fillStandartChessField(std::vector<std::vector<QString>> &chessFields)
{
    chessFields[0][4] = "wK";
    chessFields[0][3] = "wQ";
    chessFields[0][0] = "wR";
    chessFields[0][7] = "wR";
    chessFields[0][2] = "wB";
    chessFields[0][5] = "wB";
    chessFields[0][6] = "wN";
    chessFields[0][1] = "wN";

    chessFields[7][4] = "bK";
    chessFields[7][3] = "bQ";
    chessFields[7][0] = "bR";
    chessFields[7][7] = "bR";
    chessFields[7][2] = "bB";
    chessFields[7][5] = "bB";
    chessFields[7][6] = "bN";
    chessFields[7][1] = "bN";

    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        chessFields[1][i] = "wP";
        chessFields[6][i] = "bP";
    }
}

void SomeConstans::fill960ChessField(std::vector<std::vector<QString>> &chessFields)
{
    std::vector<qint8> numPos{0, 1, 2, 3, 4, 5, 6, 7};
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> randomBishop(1, 4);
    std::uniform_int_distribution<> randomQueen(0, 5);
    std::uniform_int_distribution<> randomKnight1(0, 4);
    std::uniform_int_distribution<> randomKnight2(0, 3);

    qint8 pos = randomBishop(gen) * 2 - 2;
    chessFields[0][pos] = "wB";
    chessFields[7][pos] = "bB";
    auto it = std::find(numPos.begin(), numPos.end(), pos);
    numPos.erase(it);

    pos = randomBishop(gen) * 2 - 1;
    chessFields[0][pos] = "wB";
    chessFields[7][pos] = "bB";
    it = std::find(numPos.begin(), numPos.end(), pos);
    numPos.erase(it);

    pos = randomQueen(gen);
    chessFields[0][numPos[pos]] = "wQ";
    chessFields[7][numPos[pos]] = "bQ";
    it = std::find(numPos.begin(), numPos.end(), numPos[pos]);
    numPos.erase(it);

    pos = randomKnight1(gen);
    chessFields[0][numPos[pos]] = "wN";
    chessFields[7][numPos[pos]] = "bN";
    it = std::find(numPos.begin(), numPos.end(), numPos[pos]);
    numPos.erase(it);

    pos = randomKnight2(gen);
    chessFields[0][numPos[pos]] = "wN";
    chessFields[7][numPos[pos]] = "bN";
    it = std::find(numPos.begin(), numPos.end(), numPos[pos]);
    numPos.erase(it);

    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        chessFields[1][i] = "wP";
        chessFields[6][i] = "bP";
    }

    chessFields[0][numPos[1]] = "wK";
    chessFields[0][numPos[0]] = "wR";
    chessFields[0][numPos[2]] = "wR";
    chessFields[7][numPos[1]] = "bK";
    chessFields[7][numPos[0]] = "bR";
    chessFields[7][numPos[2]] = "bR";
}

const std::vector<QString> &SomeConstans::getPieceKeysWhite() const
{
    return m_pieceKeysWhite;
}

const std::vector<QString> &SomeConstans::getPieceKeysBlack() const
{
    return m_pieceKeysBlack;
}

const std::vector<std::string> &SomeConstans::getTypeGameStr() const
{
    return m_typeGameStr;
}

const std::vector<std::string> &SomeConstans::getTypeChessStr() const
{
    return m_typeChessStr;
}

const std::vector<std::string> &SomeConstans::getTypeTimeChessStr() const
{
    return m_typeTimeChessStr;
}

const std::vector<std::vector<std::string>> &SomeConstans::getTimeChessStr() const
{
    return m_timeChessStr;
}

const std::vector<std::vector<std::pair<qint32, qint8>>> &SomeConstans::getTimeChessValue() const
{
    return m_timeChessValue;
}
