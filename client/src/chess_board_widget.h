#ifndef CHESS_BOARD_WIDGET_H
#define CHESS_BOARD_WIDGET_H

#include "src/event_button.h"
#include "src/style_lib.h"

#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QObject>
#include <QWidget>

class ChessBoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoardWidget(StyleLib *styleLib, QWidget *parent = nullptr);

    void checkField(qint8 i, qint8 j);
    void moveField(qint8 i, qint8 j);
    void baseField(qint8 i, qint8 j);
    void whiteField(qint8 i, qint8 j);

    void turnBoard();
    void turnSecondPlayer();

    void fillBoard(bool turnBoard);
    void fillIcan(bool white, bool up);

    void setIcon(qint8 i, qint8 j, QString piece);
    void setIconSize(qint8 i, qint8 j, float koef = 1);

    bool getTurnBoard();
    bool getTurnSecondPlayer();

signals:
    void clickField(QString field);

private:
    void fillFullIcans();

private:
    StyleLib *m_styleLib;

    QGridLayout *m_mainLayout;

    std::unordered_map<QString, QIcon> m_imagesOfPieces;
    std::vector<std::vector<EventButton *>> m_chessBoardBut;
    std::vector<std::vector<QLabel *>> m_otherBoardLab;

    bool m_turnBoard;
    bool m_turnSecondPlayer;
};

#endif // CHESS_BOARD_WIDGET_H
