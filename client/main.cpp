#include "game_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GameWindow::GameParams params;
    params.chessType = TypeChess::BULLET;
    params.gameType = TypeGame::OFFLINE;
    params.mainTime = {1, 10};
    params.minorTime = 5;

    GameWindow w(params);
    w.show();

    return a.exec();
}
