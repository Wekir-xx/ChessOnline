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

    m_startGameWindow = new StartGameWindow();
    m_gameWindow = new GameWindow();
    m_stacked = new QStackedWidget();

    m_stacked->addWidget(m_startGameWindow);
    m_stacked->addWidget(m_gameWindow);
    m_stacked->setCurrentWidget(m_startGameWindow);

    this->setCentralWidget(m_stacked);

    connect(m_startGameWindow, &StartGameWindow::startGame, this, [this]() {
        m_params.startParams = m_startGameWindow->getStartParams();
        m_gameWindow->startGame(m_params);

        m_stacked->setCurrentWidget(m_gameWindow);
    });

    connect(m_gameWindow, &GameWindow::exitGame, this, [this]() { m_stacked->setCurrentWidget(m_startGameWindow); });
}
