#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(pathGeneral + "avatar.png"));
    this->setStyleSheet("background-color: #1e1e1e;");

    SettingsWindow::SettingsParams settingParams;
    settingParams.checkAutoQueen = false;
    settingParams.checkPremove = false;
    settingParams.checkNoticeTime = false;
    settingParams.gameType = TypeGame::OFFLINE;

    GameWindow::GameParams params;
    params.settingParams = settingParams;
    params.timeChessType = TypeTimeChess::OTHER;
    params.mainTime = {1, 10};
    params.minorTime = 5;
    params.chessType = TypeChess::STANDART;

    m_gameWindow = new GameWindow(params);

    this->setCentralWidget(m_gameWindow);
}
