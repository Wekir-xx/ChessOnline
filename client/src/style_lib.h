#ifndef STYLE_LIB_H
#define STYLE_LIB_H

#include <QObject>

class StyleLib : public QObject
{
    Q_OBJECT

public:
    explicit StyleLib(QObject *parent = nullptr);

    void setIdIconStyle(qint8 id);
    void setIdBoardStyle(qint8 id);
    void setIdWindowStyle(qint8 id);

    qint8 getIdIconStyle();
    qint8 getIdBoardStyle();
    qint8 getIdWindowStyle();

    const std::vector<QString> &getIconStyles();
    const std::vector<QString> &getBoardStyles();
    const std::vector<QString> &getWindowStyles();

    QString getIconStyle();

    const QString &getBoardStyle() const;
    QString getBaseLightFieldStyle();
    QString getBaseDarkFieldStyle();
    QString getMoveLightFieldStyle();
    QString getMoveDarkFieldStyle();
    QString getCheckFieldStyle();
    QString getWhiteFieldStyle();

    const QString &getWindowStyle() const;
    QString getColorWindowStyle();
    QString getColorTextStyle();
    QString getButtonStyle();
    QString getComboBoxStyle();
    QString getSpinBoxStyle();
    QString getCheckBoxStyle();
    QString getMessageBoxStyle();
    QString getSecondWindowStyle();
    QString getReviewButtonStyle();

    const QString &getSmallTimeNoticeStyle() const;
    const QString &getFontSizeStyle() const;

signals:
    void changeIconStyle();
    void changeBoardStyle();
    void changeWindowStyle();

private:
    const std::vector<QString> m_iconStyles = {"style1", "style2", "style3"};

    const std::vector<QString> m_boardStyles = {"standart", "wood", "metalic"};
    const std::vector<QString> m_baseLightFieldStyles = {"EBECD0", "f0d9b5", "dee3e6"};
    const std::vector<QString> m_baseDarkFieldStyles = {"739552", "b58863", "516f85"};
    const std::vector<QString> m_moveLightFieldStyles = {"F5F682", "ffd899", "f3faac"};
    const std::vector<QString> m_moveDarkFieldStyles = {"B9CA43", "c7793a", "9cc1db"};
    const QString m_checkFieldStyle = "ff3838";
    const QString m_whiteFieldStyle = "ffffff";

    const std::vector<QString> m_windowStyles = {"white", "black", "water"};
    const std::vector<QString> m_colorWindowStyles = {"cfcfcf", "2B2B2B", "66a5ad"};
    const std::vector<QString> m_colorTextStyles = {"000", "C4C4C4", "cfcfcf"};
    const std::vector<std::vector<QString>> m_buttonStyles = {
        {"b0b0b0", "999", "8a8a8a"},
        {"383838", "636363", "212121"},
        {"07575b", "097E85", "003b46"}
    };
    const std::vector<std::vector<QString>> m_checkBoxStyles = {
        {"7152FF", "5034C9"},
        {"6049BF", "7D69D6"},
        {"749596", "9BC1C2"}
    };
    QString m_reviewButtonStyle
        = "QPushButton { background: #30C722; border: 1px solid #808080; border-radius: 4px; padding:3px 3px; }"
          "QPushButton:hover { background: #25A619; border: 1px solid #808080; border-radius: 4px; padding:3px 3px; }";

    const QString m_smallTimeNoticeStyle = "background: red; color: black; font-size:24px; padding:10px;";
    const QString m_fontSizeStyle = "font-size: 14px;";

    QString m_backgroundStyle = "background: #%1;";
    QString m_colorStyle = "color: #%1;";
    QString m_borderNone = "border: none;";

    QString m_buttonStyle =
        "QPushButton { background: #%1; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; }"
        "QPushButton:hover { background: #%2; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; }"
        "QPushButton:disabled { background: #%3; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; }";
    QString m_comboBoxStyle =
        "QComboBox { background: #%1; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; }"
        "QComboBox:hover { background: #%2; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; }"
        "QAbstractItemView { background: #%1; border: 1px solid #808080; border-radius: 4px; padding:4px 4px; outline: 0px; }"
        "QAbstractItemView::item:selected { background: #%1; color: #%3; }"
        "QAbstractItemView::item:hover { background: #%2; color: #%3; border: 1px solid #808080; }";
    QString m_spinBoxStyle = "QSpinBox { background: #%1; }";
    QString m_checkBoxStyle =
        "QCheckBox::indicator { width: 16px; height: 16px; }"
        "QCheckBox::indicator:unchecked { background:#%1; border: 1px solid #808080; border-radius: 3px; }"
        "QCheckBox::indicator:unchecked:hover { background: #%2; border: 1px solid #808080; border-radius: 3px; }"
        "QCheckBox::indicator:checked { background: #%3; border: 1px solid #808080; border-radius: 3px;"
        "image: url(:/src/images/checkmark.png); }"
        "QCheckBox::indicator:checked:hover { background: #%4; border: 1px solid #808080; border-radius: 3px; }";
    QString m_messageBoxStyle =
        "QMessageBox { background: #%1; color: #%2; }"
        "QMessageBox QPushButton { background: #%3; border: 1px solid #808080; padding: 4px 4px; }"
        "QMessageBox QPushButton:hover { background: #%4; }";
    QString m_secondWindowStyle =
        "SettingsWindow { background: #%1; color: #%2; border: 2px solid #808080; border-radius: 10px; }"
        "SettingsGameWindow { background: #%1; color: #%2; border: 2px solid #808080; border-radius: 10px; }"
        "EndGameWindow { background: #%1; color: #%2; border: 2px solid #808080; border-radius: 10px; }";

    qint8 m_idIconStyle{0};
    qint8 m_idBoardStyle{0};
    qint8 m_idWindowStyle{0};
};

#endif // STYLE_LIB_H
