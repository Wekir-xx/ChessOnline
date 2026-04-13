#ifndef END_GAME_WINDOW_H
#define END_GAME_WINDOW_H

#include "defines.h"
#include "src/player_info_layout2.h"

#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>

class EndGameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit EndGameWindow(QWidget *parent = nullptr);

    void setParams(PlayerParams params);
    void setResult(ResultGame result, std::pair<qint16, qint16> newRatings = {0, 0});

signals:
    void newGameSignal();
    void rematchSignal();
    void blockUserSignal();
    void gameReviewSignal();
    void exitSignal();

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_playerInfo;
    QHBoxLayout *m_resultLayout;
    PlayerInfoLayout2 *m_whitePlayer;
    PlayerInfoLayout2 *m_blackPlayer;
    QHBoxLayout *m_topLayout;
    QHBoxLayout *m_buttonsLayout;

    QPushButton *m_exit;
    QPushButton *m_gameReview;
    QPushButton *m_newGame;
    QPushButton *m_rematch;
    QPushButton *m_blockUser;

    QLabel *m_playerWhiteInfo;
    QLabel *m_playerBlackInfo;
    QLabel *m_newRatingsWhite;
    QLabel *m_newRatingsBlack;
    QLabel *m_result;
    QLabel *m_iconPlayerWhite;
    QLabel *m_iconPlayerBlack;
    QLabel *m_iconGame;
};

#endif // END_GAME_WINDOW_H
