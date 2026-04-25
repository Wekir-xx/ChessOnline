#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "src/chess_board.h"
#include "src/board_hlayout2.h"
#include "src/end_game_window.h"
#include "src/player_vlayout3.h"
#include "src/rotatable_label.h"
#include "src/settings_game_window.h"

#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(StyleLib *styleLib, SettingsParams &settingsParams, QWidget *parent = nullptr);

    void startGame(StartParams &startParams, ChessBoardParams &boardParams);

    SettingsParams &getSettingsParams();

signals:
    void exitGame();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void showSettingWindow();
    void showEndGameWindow();

    void startGameInner();
    void endGame(ResultGame result);
    void newGame();
    void rematch();

    void hideAll();
    void turnBoard();
    void turnSecondPlayer();
    void turnPlayerLayout(QLabel *iconPlayer, RotatableLabel *infoPlayer,
                          RotatableLabel *timePlayer, QPixmap pixmap, bool up);

    void tick();
    void setTime(qint32 seconds, bool white);
    void resetTime();

    void setStyle();

private:
    StyleLib *m_styleLib;
    ChessBoard *m_board;
    SettingsGameWindow *m_settings;
    EndGameWindow *m_endGame;
    QWidget *m_sideWidget;
    QMessageBox *m_messageBox;

    BoardHLayout2 *m_mainLayout;
    QVBoxLayout *m_sideLayout;
    QHBoxLayout *m_helperLayout;

    QPushButton *m_leftChessHistory;
    QPushButton *m_rightChessHistory;
    QPushButton *m_settingBut;
    QPushButton *m_upBut;
    QPushButton *m_downBut;
    QMetaObject::Connection m_upConnectBut;
    QMetaObject::Connection m_downConnectBut;

    std::pair<QWidget *, QWidget *> m_players;
    std::pair<PlayerVLayout3 *, PlayerVLayout3 *> m_playersLayout;
    std::pair<QPixmap, QPixmap> m_pixmapPlayers;
    std::pair<QLabel *, QLabel *> m_iconPlayers;
    std::pair<RotatableLabel *, RotatableLabel *> m_infoPlayers;
    std::pair<RotatableLabel *, RotatableLabel *> m_timePlayers;

    QTimer *m_timer;
    std::pair<qint32, qint32> m_time;
    QDateTime m_startMove;
    qint32 m_saveTime;

    SettingsParams m_settingsParams;
    StartParams m_startParams;
    ChessBoardParams m_boardParams;
    std::pair<QString, QString> m_nicknames;
    std::pair<qint16, qint16> m_ratings;
    QString m_smallTimeNoticeStyle;
    bool m_connection;
};

#endif // GAME_WINDOW_H
