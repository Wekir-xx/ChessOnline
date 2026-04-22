#include "style_lib.h"

StyleLib::StyleLib(QObject *parent)
    : QObject{parent}
{}

void StyleLib::setIconStyle(qint8 id)
{
    m_idIconStyle = id;
    emit changeIconStyle();
}

void StyleLib::setBoardStyle(qint8 id)
{
    m_idBoardStyle = id;
    emit changeBoardStyle();
}

void StyleLib::setWindowStyle(qint8 id)
{
    m_idWindowStyle = id;
    emit changeWindowStyle();
}

const std::vector<QString> &StyleLib::getIconStyles()
{
    return m_iconStyles;
}

const std::vector<QString> &StyleLib::getBoardStyles()
{
    return m_boardStyles;
}

const std::vector<QString> &StyleLib::getBaseLightFieldStyles()
{
    return m_baseLightFieldStyle;
}

const std::vector<QString> &StyleLib::getBaseDarkFieldStyles()
{
    return m_baseDarkFieldStyle;
}

const std::vector<QString> &StyleLib::getMoveLightFieldStyles()
{
    return m_moveLightFieldStyle;
}

const std::vector<QString> &StyleLib::getMoveDarkFieldStyles()
{
    return m_moveDarkFieldStyle;
}

QString StyleLib::getIconStyle()
{
    return GENERAL_PATH + m_iconStyles[m_idIconStyle] + "/";
}

const QString &StyleLib::getBoardStyle() const
{
    return m_boardStyles[m_idBoardStyle];
}

QString StyleLib::getBaseLightFieldStyle()
{
    return m_fieldStyle.arg(m_baseLightFieldStyle[m_idBoardStyle]);
}

QString StyleLib::getBaseDarkFieldStyle()
{
    return m_fieldStyle.arg(m_baseDarkFieldStyle[m_idBoardStyle]);
}

QString StyleLib::getMoveLightFieldStyle()
{
    return m_fieldStyle.arg(m_moveLightFieldStyle[m_idBoardStyle]);
}

QString StyleLib::getMoveDarkFieldStyle()
{
    return m_fieldStyle.arg(m_moveDarkFieldStyle[m_idBoardStyle]);
}

QString StyleLib::getCheckFieldStyle()
{
    return m_fieldStyle.arg(m_checkFieldStyle);
}

QString StyleLib::getWhiteFieldStyle()
{
    return m_fieldStyle.arg(m_whiteFieldStyle);
}

const QString &StyleLib::getSmallTimeNoticeStyle() const
{
    return m_smallTimeNoticeStyle;
}

const QString &StyleLib::getFontSizeStyle() const
{
    return m_fontSizeStyle;
}
