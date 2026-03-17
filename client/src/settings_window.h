#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "defines.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    struct SettingsParams
    {
        bool checkAutoQueen;
        bool checkPremove;
        bool checkNoticeTime;
        TypeGame gameType;
    };

    explicit SettingsWindow(SettingsParams params, QWidget *parent = nullptr);

signals:
    void turnBoard();
    void turnChess();
    void autoQueen();
    void premove();
    void noticeTime();
    void exitSignal();
};

#endif // SETTINGS_WINDOW_H
