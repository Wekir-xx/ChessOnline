#ifndef BOARD_SETUP_WINDOW_H
#define BOARD_SETUP_WINDOW_H

#include "src/defines.h"

#include <QObject>
#include <QWidget>

class BoardSetupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BoardSetupWindow(ChessBoardParams &boardParams, QWidget *parent = nullptr);

    ChessBoardParams &getBoardParams();

signals:
    void saveBoardParams();
    void notSaveBoardParams();

private:
    ChessBoardParams m_boardParams;
};

#endif // BOARD_SETUP_WINDOW_H
