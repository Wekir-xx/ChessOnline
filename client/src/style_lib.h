#ifndef STYLE_LIB_H
#define STYLE_LIB_H

#include "defines.h"

#include <QObject>

class StyleLib : public QObject
{
    Q_OBJECT

public:
    explicit StyleLib(QObject *parent = nullptr);

    void setIconStyle(qint8 id);
    void setBoardStyle(qint8 id);
    void setWindowStyle(qint8 id);

    const std::vector<QString> &getIconStyles();

    const std::vector<QString> &getBoardStyles();
    const std::vector<QString> &getBaseLightFieldStyles();
    const std::vector<QString> &getBaseDarkFieldStyles();
    const std::vector<QString> &getMoveLightFieldStyles();
    const std::vector<QString> &getMoveDarkFieldStyles();

    QString getIconStyle();

    const QString &getBoardStyle() const;
    QString getBaseLightFieldStyle();
    QString getBaseDarkFieldStyle();
    QString getMoveLightFieldStyle();
    QString getMoveDarkFieldStyle();
    QString getCheckFieldStyle();
    QString getWhiteFieldStyle();

    const QString &getSmallTimeNoticeStyle() const;
    const QString &getFontSizeStyle() const;

signals:
    void changeIconStyle();
    void changeBoardStyle();
    void changeWindowStyle();

private:
    const std::vector<QString> m_iconStyles = {"style1", "style2", "style3"};

    const std::vector<QString> m_boardStyles = {"standart", "wood", "metalic"};
    const std::vector<QString> m_baseLightFieldStyle = {"EBECD0", "f0d9b5", "dee3e6"};
    const std::vector<QString> m_baseDarkFieldStyle = {"739552", "b58863", "516f85"};
    const std::vector<QString> m_moveLightFieldStyle = {"F5F682", "ffd899", "f3faac"};
    const std::vector<QString> m_moveDarkFieldStyle = {"B9CA43", "c7793a", "9cc1db"};
    const QString m_checkFieldStyle = "ff3838";
    const QString m_whiteFieldStyle = "FFFFFF";
    QString m_fieldStyle = "background-color: #%1; border: none;";

    const QString m_smallTimeNoticeStyle = "background-color: red; color: black; font-size:24px; padding:10px;";
    const QString m_fontSizeStyle = "font-size: 14px;";

    qint8 m_idIconStyle{0};
    qint8 m_idBoardStyle{0};
    qint8 m_idWindowStyle{0};
};

#endif // STYLE_LIB_H
