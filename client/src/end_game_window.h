#ifndef END_GAME_WINDOW_H
#define END_GAME_WINDOW_H

#include "src/player_vlayout2.h"
#include "src/style_lib.h"
#include "src/defines.h"

#include <QLabel>
#include <QObject>
#include <QPushButton>

class EndGameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EndGameWindow(StyleLib *styleLib, QWidget *parent = nullptr);

    void setParams(PlayerParams params, TypeTimeChess timeChessType);
    void setResult(ResultGame result, std::pair<qint16, qint16> newRatings = {0, 0});

signals:
    void newGameSignal();
    void rematchSignal();
    void blockUserSignal();
    void gameReviewSignal();
    void exitSignal();

private:
    void setStyle();

private:
    StyleLib *m_styleLib;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_playerInfo;
    QHBoxLayout *m_resultLayout;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_buttonsLayout;
    PlayerVLayout2 *m_whitePlayerLayout;
    PlayerVLayout2 *m_blackPlayerLayout;
    QWidget *m_whitePlayer;
    QWidget *m_blackPlayer;

    QPushButton *m_exitBut;
    QPushButton *m_gameReviewBut;
    QPushButton *m_newGameBut;
    QPushButton *m_rematchBut;
    QPushButton *m_blockUserBut;
    QLabel *m_playerWhiteInfo;
    QLabel *m_playerBlackInfo;
    QLabel *m_newRatingsWhite;
    QLabel *m_newRatingsBlack;
    QLabel *m_result;
    QLabel *m_iconPlayerWhite;
    QLabel *m_iconPlayerBlack;
    QLabel *m_iconGame;

    bool m_whiteTurn;
};

#endif // END_GAME_WINDOW_H
