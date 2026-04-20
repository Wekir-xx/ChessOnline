#ifndef STYLE_LIB_H
#define STYLE_LIB_H

#include "defines.h"

#include <QObject>

class StyleLib : public QObject
{
    Q_OBJECT

public:
    explicit StyleLib(QObject *parent = nullptr);

    void setStyleIcon(size_t id);

    const std::vector<QString> &getStylesIcon();

    QString getStyleIcon() const;
    QString getSmallTimeNoticeStyle() const;
    QString getFontSizeStyle() const;

signals:
    void changeIconStyle();
    void changeWindowStyle();

private:
    const std::vector<QString> m_iconStyles = {"style1"};
    const QString m_smallTimeNoticeStyle = "background-color: red; color: black; font-size:24px; padding:10px;";
    const QString m_fontSizeStyle = "font-size: 14px;";

    size_t m_idStyleIcon{0};
    size_t m_idStyleWindow{0};
};

#endif // STYLE_LIB_H
