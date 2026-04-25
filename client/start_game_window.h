#ifndef STARTGAMEWINDOW_H
#define STARTGAMEWINDOW_H

#include "src/button_complex.h"
#include "src/time_chess.h"
#include "src/defines.h"
#include "src/style_lib.h"
#include "src/settings_window.h"

#include <QLabel>
#include <QStackedWidget>
#include <QResizeEvent>

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
    void settingStyles();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void showSettingWindow();
    void lookBoardSetupBut();
    void setStyle();

private:
    StyleLib *m_styleLib;
    SettingsWindow *m_settings;
    SomeConstans *m_constans;

    StartParams m_params;
    std::pair<qint8, qint8> m_saveIdTime;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_buttonLayout;
    QStackedWidget *m_stackedTime;

    ButtonComplex *m_gameTypeButs;
    ButtonComplex *m_chessTypeButs;
    ButtonComplex *m_timeChessTypeButs;
    ButtonComplex *m_timeChessButs;
    QPushButton *m_settingBut;
    QPushButton *m_boardSetupBut;
    QPushButton *m_startGameBut;
    TimeChess *m_timeChessSpins;
    QLabel *m_errorLabel;
};

#endif // STARTGAMEWINDOW_H
