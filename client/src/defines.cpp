#include "defines.h"

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

const QString &SomeConstans::getConfigFile() const
{
    return m_configFile;
}

const QString &SomeConstans::getPathGeneral() const
{
    return m_pathGeneral;
}

const QString &SomeConstans::getPathStyle1() const
{
    return m_pathStyle1;
}

const QString &SomeConstans::getSmallTimeNoticeStyle() const
{
    return m_smallTimeNoticeStyle;
}

const QString &SomeConstans::getFontSizeStyle() const
{
    return m_fontSizeStyle;
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
