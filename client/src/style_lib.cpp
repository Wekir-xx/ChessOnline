#include "style_lib.h"

#include "defines.h"

StyleLib::StyleLib(QObject *parent)
    : QObject{parent}
{}

void StyleLib::setIdIconStyle(qint8 id)
{
    m_idIconStyle = id;
    emit changeIconStyle();
}

void StyleLib::setIdBoardStyle(qint8 id)
{
    m_idBoardStyle = id;
    emit changeBoardStyle();
}

void StyleLib::setIdWindowStyle(qint8 id)
{
    m_idWindowStyle = id;
    emit changeWindowStyle();
}

qint8 StyleLib::getIdIconStyle()
{
    return m_idIconStyle;
}

qint8 StyleLib::getIdBoardStyle()
{
    return m_idBoardStyle;
}

qint8 StyleLib::getIdWindowStyle()
{
    return m_idWindowStyle;
}

const std::vector<QString> &StyleLib::getIconStyles()
{
    return m_iconStyles;
}

const std::vector<QString> &StyleLib::getBoardStyles()
{
    return m_boardStyles;
}

const std::vector<QString> &StyleLib::getWindowStyles()
{
    return m_windowStyles;
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
    return m_backgroundStyle.arg(m_baseLightFieldStyles[m_idBoardStyle]) + m_borderNone;
}

QString StyleLib::getBaseDarkFieldStyle()
{
    return m_backgroundStyle.arg(m_baseDarkFieldStyles[m_idBoardStyle]) + m_borderNone;
}

QString StyleLib::getMoveLightFieldStyle()
{
    return m_backgroundStyle.arg(m_moveLightFieldStyles[m_idBoardStyle]) + m_borderNone;
}

QString StyleLib::getMoveDarkFieldStyle()
{
    return m_backgroundStyle.arg(m_moveDarkFieldStyles[m_idBoardStyle]) + m_borderNone;
}

QString StyleLib::getCheckFieldStyle()
{
    return m_backgroundStyle.arg(m_checkFieldStyle) + m_borderNone;
}

QString StyleLib::getWhiteFieldStyle()
{
    return m_backgroundStyle.arg(m_whiteFieldStyle) + m_borderNone;
}

const QString &StyleLib::getWindowStyle() const
{
    return m_windowStyles[m_idWindowStyle];
}

QString StyleLib::getColorWindowStyle()
{
    return m_backgroundStyle.arg(m_colorWindowStyles[m_idWindowStyle]);
}

QString StyleLib::getColorTextStyle()
{
    return m_colorStyle.arg(m_colorTextStyles[m_idWindowStyle]);
}

QString StyleLib::getButtonStyle()
{
    return m_buttonStyle.arg(m_buttonStyles[m_idWindowStyle][0],
                             m_buttonStyles[m_idWindowStyle][1],
                             m_buttonStyles[m_idWindowStyle][2]);
}

QString StyleLib::getComboBoxStyle()
{
    return m_comboBoxStyle.arg(m_buttonStyles[m_idWindowStyle][0],
                               m_buttonStyles[m_idWindowStyle][1],
                               m_colorTextStyles[m_idWindowStyle]);
}

QString StyleLib::getSpinBoxStyle()
{
    return m_spinBoxStyle.arg(m_buttonStyles[m_idWindowStyle][0]);
}

QString StyleLib::getCheckBoxStyle()
{
    return m_checkBoxStyle.arg(m_buttonStyles[m_idWindowStyle][0], m_buttonStyles[m_idWindowStyle][1],
                               m_checkBoxStyles[m_idWindowStyle][0], m_checkBoxStyles[m_idWindowStyle][1]);
}

QString StyleLib::getMessageBoxStyle()
{
    return m_messageBoxStyle.arg(m_colorWindowStyles[m_idWindowStyle], m_colorTextStyles[m_idWindowStyle],
                                 m_buttonStyles[m_idWindowStyle][0], m_buttonStyles[m_idWindowStyle][1]);
}

QString StyleLib::getSecondWindowStyle()
{
    return m_secondWindowStyle.arg(m_colorWindowStyles[m_idWindowStyle], m_colorTextStyles[m_idWindowStyle]);
}

QString StyleLib::getReviewButtonStyle()
{
    return m_reviewButtonStyle;
}

const QString &StyleLib::getSmallTimeNoticeStyle() const
{
    return m_smallTimeNoticeStyle;
}

const QString &StyleLib::getFontSizeStyle() const
{
    return m_fontSizeStyle;
}
