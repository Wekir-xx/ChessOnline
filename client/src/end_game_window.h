#ifndef END_GAME_WINDOW_H
#define END_GAME_WINDOW_H

#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "../defines.h"
#include "src/player_info_layout.h"

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
        TypeChess type;
    };

    explicit EndGameWindow(PlayerParams params, QWidget *parent = nullptr);

    void setResult(ResultGame result, std::pair<qint16, qint16> newRatings = {0, 0});

signals:
    void exitSignal();
};

#endif // END_GAME_WINDOW_H
