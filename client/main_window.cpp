#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(pathGeneral + "avatar.png"));

    GameWindow::GameParams params;
    params.timeChessType = TypeTimeChess::OTHER;
    params.gameType = TypeGame::OFFLINE;
    params.mainTime = {1, 10};
    params.minorTime = 5;
    params.chessType = TypeChess::STANDART;

    m_gameWindow = new GameWindow(params);

    this->setCentralWidget(m_gameWindow);
}
