#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(pathGeneral + "avatar.png"));

    SettingsParams settingParams;
    settingParams.checkAutoQueen = false;
    settingParams.checkAutoRotate = false;
    settingParams.checkPremove = false;
    settingParams.checkNoticeTime = false;

    m_params.settingsParams = settingParams;

    m_startGameWindow = new StartGameWindow(this);
    m_gameWindow = new GameWindow(this);

    m_gameWindow->hide();

    connect(m_startGameWindow, &StartGameWindow::startGame, this, [this]() {
        m_params.startParams = m_startGameWindow->getStartParams();
        m_gameWindow->startGame(m_params);

        m_startGameWindow->hide();
        m_gameWindow->show();
        this->setCentralWidget(m_gameWindow);
    });

    this->setCentralWidget(m_startGameWindow);
}
