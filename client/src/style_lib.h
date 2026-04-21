#ifndef STYLE_LIB_H
#define STYLE_LIB_H

#include "defines.h"

#include <QObject>

class StyleLib : public QObject
{
    Q_OBJECT

public:
    explicit StyleLib(QObject *parent = nullptr);

    void setStyleIcon(qint8 id);
    void setStyleBoard(qint8 id);
    void setStyleWindow(qint8 id);

    const std::vector<QString> &getStylesIcon();

    QString getStyleIcon();

    QString getSmallTimeNoticeStyle();
    QString getFontSizeStyle();

signals:
    void changeIconStyle();
    void changeBoardStyle();
    void changeWindowStyle();

private:
    const std::vector<QString> m_iconStyles = {"style1", "style2", "style3"};
    const QString m_smallTimeNoticeStyle = "background-color: red; color: black; font-size:24px; padding:10px;";
    const QString m_fontSizeStyle = "font-size: 14px;";

    qint8 m_idStyleIcon{0};
    qint8 m_idStyleBoard{0};
    qint8 m_idStyleWindow{0};
};

#endif // STYLE_LIB_H
