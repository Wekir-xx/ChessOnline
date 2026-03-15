#ifndef END_GAME_WINDOW_H
#define END_GAME_WINDOW_H

#include "defines.h"
#include "src/player_info_layout.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QWidget>

class EndGameWindow : public QWidget
{
    Q_OBJECT

public:

    struct PlayerParams
    {
        std::pair<QString, QString> nicknames;
        std::pair<QPixmap, QPixmap> icons;
        std::pair<qint16, qint16> ratings;
        bool mainPlayerWhite;
        TypeTimeChess type;
    };

    explicit EndGameWindow(PlayerParams params, QWidget *parent = nullptr);

    void setResult(ResultGame result, std::pair<qint16, qint16> newRatings = {0, 0});

signals:
    void newGameSignal();
    void rematchSignal();
    void blockUserSignal();
    void gameReviewSignal();
    void exitSignal();

private:
    QLabel *m_newRatingsWhite;
    QLabel *m_newRatingsBlack;
    QLabel *m_result;
};

#endif // END_GAME_WINDOW_H
