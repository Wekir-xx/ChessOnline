#include "style_lib.h"

StyleLib::StyleLib(QObject *parent)
    : QObject{parent}
{}

void StyleLib::setStyleIcon(qint8 id)
{
    m_idStyleIcon = id;
    emit changeIconStyle();
}

const std::vector<QString> &StyleLib::getStylesIcon()
{
    return m_iconStyles;
}

QString StyleLib::getStyleIcon() const
{
    return GENERAL_PATH + m_iconStyles[m_idStyleIcon] + "/";
}

QString StyleLib::getSmallTimeNoticeStyle() const
{
    return m_smallTimeNoticeStyle;
}

QString StyleLib::getFontSizeStyle() const
{
    return m_fontSizeStyle;
}
