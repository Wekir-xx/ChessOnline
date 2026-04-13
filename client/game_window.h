#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "chess_board.h"
#include "src/end_game_window.h"
#include "src/settings_window.h"
#include "src/player_info_layout3.h"
#include "src/rotatable_label.h"

#include <QDateTime>
#include <QTimer>

class GameWindow : public QWidget
{
public:
    explicit GameWindow(QWidget *parent = nullptr);

    void startGame(GameParams &params);

    void showSettingWindow();
    void showEndGameWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void startGameInner(bool first = false);
    void endGame(ResultGame result);
    void newGame();
    void rematch();

    void turnBoard();
    void turnWidget(QVBoxLayout *layout, QWidget *widget1, QWidget *widget2);
    void turnSecondPlayer();
    void turnPlayerLayout(QLabel *iconPlayer, RotatableLabel *infoPlayer,
                          RotatableLabel *timePlayer, QPixmap pixmap, bool up);

    void tick();
    void setTime(qint32 seconds, bool white);
    void resetTime();

private:
    ChessBoard *m_board;
    SettingsWindow *m_settings;
    EndGameWindow *m_endGame;
    QVBoxLayout *m_sideLayout;

    std::pair<QWidget *, QWidget *> m_players;
    std::pair<PlayerInfoLayout3 *, PlayerInfoLayout3 *> m_playersLayout;
    std::pair<QPixmap, QPixmap> m_pixmapPlayers;
    std::pair<QLabel *, QLabel *> m_iconPlayers;
    std::pair<RotatableLabel *, RotatableLabel *> m_infoPlayers;
    std::pair<RotatableLabel *, RotatableLabel *> m_timePlayers;

    QTimer *m_timer;
    std::pair<qint32, qint32> m_time;
    QDateTime m_startMove;
    qint32 m_saveTime;

    QPushButton *m_upButton;
    QPushButton *m_downButton;
    QMetaObject::Connection m_upButtonCon;
    QMetaObject::Connection m_downButtonCon;

    StartParams m_StartParams;
    SettingsParams m_SettingsParams;
    std::pair<QString, QString> m_nicknames;
    std::pair<qint16, qint16> m_ratings;
};

#endif // GAME_WINDOW_H
