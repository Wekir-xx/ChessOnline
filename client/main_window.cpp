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
    this->readChessBoardParams();

    m_startGameWindow = new StartGameWindow(m_params.startParams);
    m_gameWindow = new GameWindow();
    m_boardSetupWindow = new BoardSetupWindow(m_params.boardParams);
    m_stacked = new QStackedWidget();

    m_stacked->addWidget(m_startGameWindow);
    m_stacked->addWidget(m_gameWindow);
    m_stacked->addWidget(m_boardSetupWindow);
    m_stacked->setCurrentWidget(m_startGameWindow);

    this->setCentralWidget(m_stacked);

    connect(m_startGameWindow, &StartGameWindow::startGame, this, [this]() {
        m_params.startParams = m_startGameWindow->getStartParams();
        m_gameWindow->startGame(m_params);
        m_stacked->setCurrentWidget(m_gameWindow);
        m_startGameWindow->hideAllWidget();
    });

    connect(m_startGameWindow, &StartGameWindow::boardSetup, this, [this]() {
        m_stacked->setCurrentWidget(m_boardSetupWindow);
        m_startGameWindow->hideAllWidget();
    });

    connect(m_gameWindow, &GameWindow::exitGame, this, [this]() {
        m_params.settingsParams = m_gameWindow->getSettingsParams();
        this->switchStartGameWindow();
    });

    connect(m_boardSetupWindow, &BoardSetupWindow::saveParams, this, [this]() {
        m_params.boardParams = m_boardSetupWindow->getBoardParams();
        this->switchStartGameWindow();
    });

    connect(m_boardSetupWindow, &BoardSetupWindow::exit, this, [this]() {
        this->switchStartGameWindow();
    });
}

MainWindow::~MainWindow()
{
    m_params.settingsParams = m_gameWindow->getSettingsParams();
    m_params.startParams = m_startGameWindow->getStartParams();
    m_params.boardParams = m_boardSetupWindow->getBoardParams();
    this->writeSettingsParams();
    this->writeStartParams();
    this->writeChessBoardParams();
}

void MainWindow::switchStartGameWindow()
{
    m_stacked->setCurrentWidget(m_startGameWindow);
    m_startGameWindow->showAllWidget();
}

void MainWindow::writeSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_settings->setValue("hideAll", m_params.settingsParams.hideAll);
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

    m_settings->endGroup();
}

void MainWindow::writeChessBoardParams()
{
    m_settings->beginGroup("Chess");

    m_settings->setValue("board", getBoard());
    m_settings->setValue("castling1", m_params.boardParams.castling.first.first);
    m_settings->setValue("castling2", m_params.boardParams.castling.first.second);
    m_settings->setValue("castling3", m_params.boardParams.castling.second.first);
    m_settings->setValue("castling4", m_params.boardParams.castling.second.second);
    m_settings->setValue("chess960", m_params.boardParams.chess960);
    m_settings->setValue("whiteMove", m_params.boardParams.whiteMove);

    m_settings->endGroup();
}

void MainWindow::readSettingsParams()
{
    m_settings->beginGroup("Setting");

    m_params.settingsParams.hideAll = m_settings->value("hideAll", m_params.settingsParams.hideAll).toBool();
    m_params.settingsParams.checkAutoQueen = m_settings->value("hideAll", m_params.settingsParams.checkAutoQueen).toBool();
    m_params.settingsParams.checkAutoRotate = m_settings->value("checkAutoRotate", m_params.settingsParams.checkAutoRotate).toBool();
    m_params.settingsParams.checkPremove = m_settings->value("checkPremove", m_params.settingsParams.checkPremove).toBool();
    m_params.settingsParams.checkNoticeTime = m_settings->value("checkNoticeTime", m_params.settingsParams.checkNoticeTime).toBool();

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
    m_params.startParams.mainTime = m_settings->value("mainTime", m_params.startParams.mainTime).toInt();
    m_params.startParams.minorTime = m_settings->value("minorTime", m_params.startParams.minorTime).toInt();

    m_settings->endGroup();
}

void MainWindow::readChessBoardParams()
{
    m_settings->beginGroup("Chess");

    setBoard(m_settings->value("board", "").toString());
    m_params.boardParams.castling.first.first = m_settings->value("castling1", m_params.boardParams.castling.first.first).toBool();
    m_params.boardParams.castling.first.second = m_settings->value("castling2", m_params.boardParams.castling.first.second).toBool();
    m_params.boardParams.castling.second.first = m_settings->value("castling3", m_params.boardParams.castling.second.first).toBool();
    m_params.boardParams.castling.second.second = m_settings->value("castling4", m_params.boardParams.castling.second.second).toBool();
    m_params.boardParams.chess960 = m_settings->value("chess960", m_params.boardParams.chess960).toBool();
    m_params.boardParams.whiteMove = m_settings->value("whiteMove", m_params.boardParams.whiteMove).toBool();

    m_settings->endGroup();
}

void MainWindow::setBoard(QString board)
{
    m_params.boardParams.chessFields.resize(SIDE_SIZE, std::vector<QString>(SIDE_SIZE));

    if (board.isEmpty()) {
        SomeConstans::fillStandartChessField(m_params.boardParams.chessFields);
    } else {
        auto it = board.begin();
        for (size_t i = 0; i < SIDE_SIZE; ++i) {
            for (size_t j = 0; j < SIDE_SIZE; ++j) {
                if (*it == '.') {
                    ++it;
                } else {
                    m_params.boardParams.chessFields[i][j] = QString(*it) + QString(*(it + 1));
                    it += 2;
                }
            }
        }
    }
}

QString MainWindow::getBoard()
{
    QString board;
    for (const auto &rows : m_params.boardParams.chessFields) {
        for (const auto &field : rows) {
            if (field.isEmpty())
                board += ".";
            else
                board += field;
        }
    }
    return board;
}
