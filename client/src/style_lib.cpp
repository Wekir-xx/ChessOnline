#include "style_lib.h"

StyleLib::StyleLib(QObject *parent)
    : QObject{parent}
{}

void StyleLib::setStyleIcon(qint8 id)
{
    m_idStyleIcon = id;
    emit changeIconStyle();
}

void StyleLib::setStyleBoard(qint8 id)
{
    m_idStyleBoard = id;
    emit changeBoardStyle();
}

void StyleLib::setStyleWindow(qint8 id)
{
    m_idStyleWindow = id;
    emit changeWindowStyle();
}

const std::vector<QString> &StyleLib::getStylesIcon()
{
    return m_iconStyles;
}

QString StyleLib::getStyleIcon()
{
    return GENERAL_PATH + m_iconStyles[m_idStyleIcon] + "/";
}

QString StyleLib::getSmallTimeNoticeStyle()
{
    return m_smallTimeNoticeStyle;
}

QString StyleLib::getFontSizeStyle()
{
    return m_fontSizeStyle;
}
