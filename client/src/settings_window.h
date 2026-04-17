#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "defines.h"

#include <QCheckBox>
#include <QPushButton>
#include <QWidget>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

    void setParams(SettingsParams params, TypeGame gameType, TypeTimeChess timeChessType);
    void setExitButton(bool flag);

signals:
    void hideAll();
    void turnBoard();
    void turnSecondPlayer();
    void autoQueen();
    void autoRotate();
    void premove();
    void noticeTime();
    void exit();
    void exitGame();

private:
    QVBoxLayout *m_layoutV;
    QHBoxLayout *m_layoutH;

    QPushButton *m_exit;
    QPushButton *m_turnBoardBut;
    QPushButton *m_turnChessBut;
    QPushButton *m_exitGame;

    QCheckBox *m_hideAllBut;
    QCheckBox *m_autoQueenBut;
    QCheckBox *m_autoRotateBut;
    QCheckBox *m_premoveBut;
    QCheckBox *m_noticeTimeBut;
};

#endif // SETTINGS_WINDOW_H
