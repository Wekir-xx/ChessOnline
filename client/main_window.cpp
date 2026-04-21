#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_styleLib = new StyleLib(this);

    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(QString(GENERAL_PATH) + "avatar.png"));

    m_settings.reset(new QSettings(CONFIG_FILE, QSettings::IniFormat));
    this->readSettingsParams();
    this->readStartParams();
    this->readChessBoardParams();

    m_startGameWindow = new StartGameWindow(m_styleLib, m_startParams);
    m_gameWindow = new GameWindow(m_styleLib, m_settingsParams);
    m_boardSetupWindow = new BoardSetupWindow(m_styleLib, m_boardParams);
    m_stacked = new QStackedWidget();

    m_stacked->addWidget(m_startGameWindow);
    m_stacked->addWidget(m_gameWindow);
    m_stacked->addWidget(m_boardSetupWindow);
    m_stacked->setCurrentWidget(m_startGameWindow);

    m_gameWindow->setFixedSize(0, 0);
    m_boardSetupWindow->setFixedSize(0, 0);
    this->setCentralWidget(m_stacked);

    connect(m_startGameWindow, &StartGameWindow::startGame, this, [this]() {
        m_startParams = m_startGameWindow->getStartParams();
        m_gameWindow->startGame(m_startParams, m_boardParams);

        m_startGameWindow->setFixedSize(0, 0);
        m_stacked->setCurrentWidget(m_gameWindow);
        m_gameWindow->setMinimumSize(0, 0);
        m_gameWindow->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    });

    connect(m_startGameWindow, &StartGameWindow::boardSetup, this, [this]() {
        m_startGameWindow->setFixedSize(0, 0);
        m_stacked->setCurrentWidget(m_boardSetupWindow);
        m_boardSetupWindow->setMinimumSize(0, 0);
        m_boardSetupWindow->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    });

    connect(m_gameWindow, &GameWindow::exitGame, this, [this]() {
        m_gameWindow->setFixedSize(0, 0);
        m_stacked->setCurrentWidget(m_startGameWindow);
        m_startGameWindow->setMinimumSize(0, 0);
        m_startGameWindow->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    });

    connect(m_boardSetupWindow, &BoardSetupWindow::saveParams, this, [this]() {
        m_boardParams = m_boardSetupWindow->getBoardParams();

        m_boardSetupWindow->setFixedSize(0, 0);
        m_stacked->setCurrentWidget(m_startGameWindow);
        m_startGameWindow->setMinimumSize(0, 0);
        m_startGameWindow->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    });

    connect(m_boardSetupWindow, &BoardSetupWindow::exit, this, [this]() {
        m_boardSetupWindow->setFixedSize(0, 0);
        m_stacked->setCurrentWidget(m_startGameWindow);
        m_startGameWindow->setMinimumSize(0, 0);
        m_startGameWindow->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    });
}

MainWindow::~MainWindow()
{
    m_settingsParams = m_gameWindow->getSettingsParams();
    m_startParams = m_startGameWindow->getStartParams();
    m_boardParams = m_boardSetupWindow->getBoardParams();
    this->writeSettingsParams();
    this->writeStartParams();
    this->writeChessBoardParams();
}

void MainWindow::writeSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_settings->setValue("hideAll", m_settingsParams.hideAll);
    m_settings->setValue("checkAutoQueen", m_settingsParams.checkAutoQueen);
    m_settings->setValue("checkAutoRotate", m_settingsParams.checkAutoRotate);
    m_settings->setValue("checkPremove", m_settingsParams.checkPremove);
    m_settings->setValue("checkNoticeTime", m_settingsParams.checkNoticeTime);

    m_settings->endGroup();
}

void MainWindow::writeStartParams()
{
    m_settings->beginGroup("Start");

    m_settings->setValue("gameType", static_cast<int>(m_startParams.gameType));
    m_settings->setValue("chessType", static_cast<int>(m_startParams.chessType));
    m_settings->setValue("timeChessType", static_cast<int>(m_startParams.timeChessType));
    m_settings->setValue("mainTime", m_startParams.mainTime);
    m_settings->setValue("minorTime", m_startParams.minorTime);

    m_settings->endGroup();
}

void MainWindow::writeChessBoardParams()
{
    m_settings->beginGroup("Chess");

    m_settings->setValue("board", getBoard());
    m_settings->setValue("castling1", m_boardParams.castling.first.first);
    m_settings->setValue("castling2", m_boardParams.castling.first.second);
    m_settings->setValue("castling3", m_boardParams.castling.second.first);
    m_settings->setValue("castling4", m_boardParams.castling.second.second);
    m_settings->setValue("chess960", m_boardParams.chess960);
    m_settings->setValue("whiteMove", m_boardParams.whiteMove);

    m_settings->endGroup();
}

void MainWindow::readSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_settingsParams.hideAll = m_settings->value("hideAll", m_settingsParams.hideAll).toBool();
    m_settingsParams.checkAutoQueen = m_settings->value("checkAutoQueen", m_settingsParams.checkAutoQueen).toBool();
    m_settingsParams.checkAutoRotate = m_settings->value("checkAutoRotate", m_settingsParams.checkAutoRotate).toBool();
    m_settingsParams.checkPremove = m_settings->value("checkPremove", m_settingsParams.checkPremove).toBool();
    m_settingsParams.checkNoticeTime = m_settings->value("checkNoticeTime", m_settingsParams.checkNoticeTime).toBool();

    m_settings->endGroup();
}

void MainWindow::readStartParams()
{
    m_settings->beginGroup("Start");

    m_startParams.gameType = static_cast<TypeGame>(m_settings->value("gameType", static_cast<int>(m_startParams.gameType)).toInt());
    m_startParams.chessType = static_cast<TypeChess>(m_settings->value("chessType", static_cast<int>(m_startParams.chessType)).toInt());
    m_startParams.timeChessType = static_cast<TypeTimeChess>(
        m_settings->value("timeChessType", static_cast<int>(m_startParams.timeChessType)).toInt());
    m_startParams.mainTime = m_settings->value("mainTime", m_startParams.mainTime).toInt();
    m_startParams.minorTime = m_settings->value("minorTime", m_startParams.minorTime).toInt();

    m_settings->endGroup();
}

void MainWindow::readChessBoardParams()
{
    m_settings->beginGroup("Chess");

    setBoard(m_settings->value("board", "").toString());
    m_boardParams.castling.first.first = m_settings->value("castling1", m_boardParams.castling.first.first).toBool();
    m_boardParams.castling.first.second = m_settings->value("castling2", m_boardParams.castling.first.second).toBool();
    m_boardParams.castling.second.first = m_settings->value("castling3", m_boardParams.castling.second.first).toBool();
    m_boardParams.castling.second.second = m_settings->value("castling4", m_boardParams.castling.second.second).toBool();
    m_boardParams.chess960 = m_settings->value("chess960", m_boardParams.chess960).toBool();
    m_boardParams.whiteMove = m_settings->value("whiteMove", m_boardParams.whiteMove).toBool();

    m_settings->endGroup();
}

void MainWindow::setBoard(QString board)
{
    m_boardParams.chessFields.resize(SIDE_SIZE, std::vector<QString>(SIDE_SIZE));

    if (board.isEmpty()) {
        SomeConstans::fillStandartChessField(m_boardParams.chessFields);
    } else {
        auto it = board.begin();
        for (qint8 i = 0; i < SIDE_SIZE; ++i) {
            for (qint8 j = 0; j < SIDE_SIZE; ++j) {
                if (*it == '.') {
                    ++it;
                } else {
                    m_boardParams.chessFields[i][j] = QString(*it) + QString(*(it + 1));
                    it += 2;
                }
            }
        }
    }
}

QString MainWindow::getBoard()
{
    QString board;
    for (const auto &rows : m_boardParams.chessFields) {
        for (const auto &field : rows) {
            if (field.isEmpty())
                board += ".";
            else
                board += field;
        }
    }
    return board;
}
