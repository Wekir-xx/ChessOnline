#ifndef CHESS_BOARD_WIDGET_H
#define CHESS_BOARD_WIDGET_H

#include "event_button.h"
#include "../backend/style_lib.h"

#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <unordered_map>

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoardWidget(StyleLib *styleLib, QWidget *parent = nullptr);

    void updateBoard();

    void checkField(int i, int j);
    void moveField(int i, int j);
    void baseField(int i, int j);
    void whiteField(int i, int j);

    void turnBoard();
    void turnSecondPlayer();

    void fillBoard(bool turnBoard);
    void fillIcan(bool white, bool up);

    void setIcon(int i, int j, QString piece);
    void setIconSize(int i, int j, float koef = 1);

    bool getTurnBoard();
    bool getTurnSecondPlayer();

signals:
    void clickField(QString field);

private:
    void fillFullIcans();
    void fillStyles();

private:
    StyleLib *m_styleLib;

    QGridLayout *m_mainLayout;

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<EventButton *>> m_chessBoardBut;
    std::vector<std::vector<QLabel *>> m_otherBoardLab;

    QString m_baseLightFieldStyle;
    QString m_baseDarkFieldStyle;
    QString m_moveLightFieldStyle;
    QString m_moveDarkFieldStyle;
    QString m_checkFieldStyle;
    QString m_whiteFieldStyle;

    bool m_turnBoard;
    bool m_turnSecondPlayer;
};

#endif // CHESS_BOARD_WIDGET_H
