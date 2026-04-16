#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(SomeConstans::getInstance().getPathGeneral() + "avatar.png"));

    m_settings.reset(new QSettings(SomeConstans::getInstance().getConfigFile(), QSettings::IniFormat));
    this->readSettingsParams();
    this->readStartParams();

    m_startGameWindow = new StartGameWindow(m_params.startParams);
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

    connect(m_gameWindow, &GameWindow::exitGame, this, [this]() {
        m_params.settingsParams = m_gameWindow->getSettingsParams();
        m_stacked->setCurrentWidget(m_startGameWindow);
    });
}

MainWindow::~MainWindow()
{
    m_params.startParams = m_startGameWindow->getStartParams();
    m_params.settingsParams = m_gameWindow->getSettingsParams();
    this->writeStartParams();
    this->writeSettingsParams();
}

void MainWindow::writeSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_settings->setValue("checkAutoQueen", m_params.settingsParams.checkAutoQueen);
    m_settings->setValue("checkAutoRotate", m_params.settingsParams.checkAutoRotate);
    m_settings->setValue("checkPremove", m_params.settingsParams.checkPremove);
    m_settings->setValue("checkNoticeTime", m_params.settingsParams.checkNoticeTime);

    m_settings->endGroup();
}

void MainWindow::writeStartParams()
{
    m_settings->beginGroup("Start");

    m_settings->setValue("gameType", static_cast<int>(m_params.startParams.gameType));
    m_settings->setValue("chessType", static_cast<int>(m_params.startParams.chessType));
    m_settings->setValue("timeChessType", static_cast<int>(m_params.startParams.timeChessType));
    m_settings->setValue("mainTime", m_params.startParams.mainTime);
    m_settings->setValue("minorTime", m_params.startParams.minorTime);

    m_settings->beginGroup("Chess");
    m_settings->setValue("board", getBoard());
    m_settings->setValue("castling1", m_params.startParams.castling.first.first);
    m_settings->setValue("castling2", m_params.startParams.castling.first.second);
    m_settings->setValue("castling3", m_params.startParams.castling.second.first);
    m_settings->setValue("castling4", m_params.startParams.castling.second.second);
    m_settings->setValue("whiteMove", m_params.startParams.whiteMove);

    m_settings->endGroup();
    m_settings->endGroup();
}

void MainWindow::readSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_params.settingsParams.checkAutoQueen = m_settings->value("checkAutoQueen",
            m_params.settingsParams.checkAutoQueen).toBool();
    m_params.settingsParams.checkAutoRotate = m_settings->value("checkAutoRotate",
            m_params.settingsParams.checkAutoRotate).toBool();
    m_params.settingsParams.checkPremove = m_settings->value("checkPremove",
                                           m_params.settingsParams.checkPremove).toBool();
    m_params.settingsParams.checkNoticeTime = m_settings->value("checkNoticeTime",
            m_params.settingsParams.checkNoticeTime).toBool();

    m_settings->endGroup();
}

void MainWindow::readStartParams()
{
    m_settings->beginGroup("Start");

    m_params.startParams.gameType = static_cast<TypeGame>(m_settings->value("gameType",
                                    static_cast<int>(m_params.startParams.gameType)).toInt());
    m_params.startParams.chessType = static_cast<TypeChess>(m_settings->value("chessType",
                                     static_cast<int>(m_params.startParams.chessType)).toInt());
    m_params.startParams.timeChessType = static_cast<TypeTimeChess>(m_settings->value("timeChessType",
                                         static_cast<int>(m_params.startParams.timeChessType)).toInt());
    m_params.startParams.mainTime = m_settings->value("mainTime",
                                    m_params.startParams.mainTime).toInt();
    m_params.startParams.minorTime = m_settings->value("minorTime",
                                     m_params.startParams.minorTime).toInt();

    m_settings->beginGroup("Chess");
    setBoard(m_settings->value("board", "").toString());
    m_params.startParams.castling.first.first = m_settings->value("castling1",
            m_params.startParams.castling.first.first).toBool();
    m_params.startParams.castling.first.second = m_settings->value("castling2",
            m_params.startParams.castling.first.second).toBool();
    m_params.startParams.castling.second.first = m_settings->value("castling3",
            m_params.startParams.castling.second.first).toBool();
    m_params.startParams.castling.second.second = m_settings->value("castling4",
            m_params.startParams.castling.second.second).toBool();
    m_params.startParams.whiteMove = m_settings->value("whiteMove",
                                     m_params.startParams.whiteMove).toBool();

    m_settings->endGroup();
    m_settings->endGroup();
}

void MainWindow::setBoard(const QString board)
{

}

const QString MainWindow::getBoard()
{
    return "";
}
