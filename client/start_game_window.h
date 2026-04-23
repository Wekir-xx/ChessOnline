#ifndef STARTGAMEWINDOW_H
#define STARTGAMEWINDOW_H

#include "src/button_complex.h"
#include "src/time_chess.h"

#include <QLabel>
#include <QStackedWidget>

class StartGameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StartGameWindow(StyleLib *styleLib, StartParams &startParams, QWidget *parent = nullptr);

    void setChessBoard();
    StartParams &getStartParams();

signals:
    void startGame();
    void boardSetup();

private:
    void lookBoardSetupBut();
    void setStyle();

private:
    StyleLib *m_styleLib;
    SomeConstans *m_constans;

    StartParams m_params;
    std::pair<qint8, qint8> m_saveIdTime;

    QVBoxLayout *m_mainLayout;
    QStackedWidget *m_stackedTime;
    QHBoxLayout *m_buttonLayout;

    ButtonComplex *m_gameTypeButs;
    ButtonComplex *m_chessTypeButs;
    ButtonComplex *m_timeChessTypeButs;
    ButtonComplex *m_timeChessButs;
    QPushButton *m_boardSetupBut;
    QPushButton *m_startGameBut;
    TimeChess *m_timeChessSpins;
    QLabel *m_errorLabel;
};

#endif // STARTGAMEWINDOW_H
