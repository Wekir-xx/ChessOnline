#ifndef STARTGAMEWINDOW_H
#define STARTGAMEWINDOW_H

#include "src/button_complex.h"
#include "src/defines.h"
#include "src/time_chess.h"

#include <QLabel>
#include <QObject>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class StartGameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartGameWindow(QWidget *parent = nullptr);

    StartParams &getStartParams();

signals:
    void startGame();

private:
    QVBoxLayout *m_mainLayout;
    QStackedWidget *m_stackedTime;

    ButtonComplex *m_gameTypeButtons;
    ButtonComplex *m_chessTypeButtons;
    ButtonComplex *m_timeChessTypeButtons;
    ButtonComplex *m_timeChessButtons;
    TimeChess *m_timeChessSpin;
    QLabel *m_errorLabel;
    QPushButton *m_startGame;

    StartParams m_params;
    SomeConstans *m_constans;
};

#endif // STARTGAMEWINDOW_H
