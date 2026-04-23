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
    return m_baseLightFieldStyles;
}

const std::vector<QString> &StyleLib::getBaseDarkFieldStyles()
{
    return m_baseDarkFieldStyles;
}

const std::vector<QString> &StyleLib::getMoveLightFieldStyles()
{
    return m_moveLightFieldStyles;
}

const std::vector<QString> &StyleLib::getMoveDarkFieldStyles()
{
    return m_moveDarkFieldStyles;
}

const std::vector<QString> &StyleLib::getWindowStyles()
{
    return m_windowStyles;
}

const std::vector<QString> &StyleLib::getColorWindowStyles()
{
    return m_colorWindowStyles;
}

const std::vector<QString> &StyleLib::getColorTextStyles()
{
    return m_colorTextStyles;
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
    return m_fieldStyle.arg(m_baseLightFieldStyles[m_idBoardStyle]);
}

QString StyleLib::getBaseDarkFieldStyle()
{
    return m_fieldStyle.arg(m_baseDarkFieldStyles[m_idBoardStyle]);
}

QString StyleLib::getMoveLightFieldStyle()
{
    return m_fieldStyle.arg(m_moveLightFieldStyles[m_idBoardStyle]);
}

QString StyleLib::getMoveDarkFieldStyle()
{
    return m_fieldStyle.arg(m_moveDarkFieldStyles[m_idBoardStyle]);
}

QString StyleLib::getCheckFieldStyle()
{
    return m_fieldStyle.arg(m_checkFieldStyle);
}

QString StyleLib::getWhiteFieldStyle()
{
    return m_fieldStyle.arg(m_whiteFieldStyle);
}

const QString &StyleLib::getWindowStyle() const
{
    return m_windowStyles[m_idWindowStyle];
}

QString StyleLib::getColorWindowStyle()
{
    return m_colorWindowStyle.arg(m_colorWindowStyles[m_idWindowStyle]);
}

QString StyleLib::getColorTextStyle()
{
    return m_colorTextStyle.arg(m_colorTextStyles[m_idWindowStyle]);
}

const QString &StyleLib::getSmallTimeNoticeStyle() const
{
    return m_smallTimeNoticeStyle;
}

const QString &StyleLib::getFontSizeStyle() const
{
    return m_fontSizeStyle;
}
