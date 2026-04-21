#include "game_window.h"

#include <QMessageBox>

GameWindow::GameWindow(StyleLib *styleLib, SettingsParams &settingsParams, QWidget *parent)
    : QWidget{parent}
    , m_settingsParams{settingsParams}
{
    m_styleLib = styleLib;
    m_connection = false;

    m_board = new ChessBoard(m_styleLib);
    m_settings = new SettingsWindow(m_styleLib, this);
    m_endGame = new EndGameWindow(m_styleLib, this);
    m_sideWidget = new QWidget();
    m_timer = new QTimer(this);

    m_mainLayout = new BoardHLayout2();
    m_sideLayout = new QVBoxLayout();
    m_helperLayout = new QHBoxLayout();

    m_leftChessHistory = new QPushButton();
    m_rightChessHistory = new QPushButton();
    m_settingsButton = new QPushButton();
    m_upBut = new QPushButton();
    m_downBut = new QPushButton();

    m_players = std::pair{new QWidget(), new QWidget()};
    m_playersLayout = std::pair{new PlayerVLayout3(), new PlayerVLayout3()};

    m_iconPlayers = std::pair{new QLabel(), new QLabel()};
    m_infoPlayers = std::pair{new RotatableLabel(), new RotatableLabel()};
    m_timePlayers = std::pair{new RotatableLabel(), new RotatableLabel()};

    m_leftChessHistory->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);
    m_rightChessHistory->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);
    m_settingsButton->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);

    m_leftChessHistory->setIcon(QIcon(QString(GENERAL_PATH) + "leftArrow.png"));
    m_rightChessHistory->setIcon(QIcon(QString(GENERAL_PATH) + "rightArrow.png"));
    m_settingsButton->setIcon(QIcon(QString(GENERAL_PATH) + "settings.png"));

    m_leftChessHistory->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));
    m_rightChessHistory->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));
    m_settingsButton->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));

    m_leftChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    m_rightChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    m_settingsButton->setStyleSheet("QPushButton { padding: 0px; }");

    m_iconPlayers.first->setScaledContents(true);
    m_iconPlayers.second->setScaledContents(true);
    m_iconPlayers.first->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_iconPlayers.second->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_infoPlayers.first->setAlignment(Qt::AlignCenter);
    m_infoPlayers.second->setAlignment(Qt::AlignCenter);
    m_timePlayers.first->setAlignment(Qt::AlignCenter);
    m_timePlayers.second->setAlignment(Qt::AlignCenter);

    m_infoPlayers.first->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_infoPlayers.second->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_timePlayers.first->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    m_timePlayers.second->setFixedHeight(FIXED_SIZE_TYPE_GAME);

    m_settings->hide();
    m_endGame->hide();

    m_board->setAutoQueen(m_settingsParams.checkAutoQueen);
    m_board->setPremove(m_settingsParams.checkPremove);

    m_players.first->setLayout(m_playersLayout.first);
    m_players.second->setLayout(m_playersLayout.second);

    m_playersLayout.first->addWidget(m_timePlayers.first);
    m_playersLayout.first->addWidget(m_infoPlayers.first);
    m_playersLayout.first->addWidget(m_iconPlayers.first);
    m_playersLayout.second->addWidget(m_iconPlayers.second);
    m_playersLayout.second->addWidget(m_infoPlayers.second);
    m_playersLayout.second->addWidget(m_timePlayers.second);
    m_playersLayout.first->swapOrientation();

    m_helperLayout->addWidget(m_leftChessHistory);
    m_helperLayout->addWidget(m_rightChessHistory);
    m_helperLayout->addWidget(m_settingsButton);

    m_sideLayout->addWidget(m_players.second, 2);
    m_sideLayout->addWidget(m_upBut, 1);
    m_sideLayout->addLayout(m_helperLayout, 1);
    m_sideLayout->addWidget(m_downBut, 1);
    m_sideLayout->addWidget(m_players.first, 2);

    m_sideWidget->setLayout(m_sideLayout);

    m_mainLayout->addWidget(m_board, 10);
    m_mainLayout->addWidget(m_sideWidget, 1);

    this->setLayout(m_mainLayout);

    connect(m_board, &ChessBoard::didMove, this, [this]() {
        if (m_settingsParams.checkAutoRotate)
            this->turnBoard();

        m_settings->setExitButton(false);

        if (m_startParams.timeChessType == TypeTimeChess::NO_TIME)
            return;

        if (!m_timer->isActive()) {
            m_startMove = QDateTime::currentDateTime();
            m_timer->start(TICK);
        }

        bool white = m_board->getColorMove();
        if (white) {
            m_time.first += m_startParams.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.first > MAX_TIME_SECONDS)
                m_time.first = MAX_TIME_SECONDS;
        } else {
            m_time.second += m_startParams.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.second > MAX_TIME_SECONDS)
                m_time.second = MAX_TIME_SECONDS;
        }
        this->setTime(0, white);
        m_startMove = QDateTime::currentDateTime();
    });
    connect(m_board, &ChessBoard::endGame, this, [this](ResultGame result) { this->endGame(result); });

    connect(m_leftChessHistory, &QPushButton::clicked, this, [this]() { m_board->historyBack(); });
    connect(m_rightChessHistory, &QPushButton::clicked, this, [this]() { m_board->historyForward(); });
    connect(m_settingsButton, &QPushButton::clicked, this, [this]() {
        if (!m_settings->isVisible()) {
            m_settings->raise();
            m_settings->show();
            this->showSettingWindow();
        } else {
            m_settings->hide();
        }
    });

    connect(m_settings, &SettingsWindow::hideAll, this, [this]() {
        m_settingsParams.hideAll ^= true;
        this->hideAll();
    });
    connect(m_settings, &SettingsWindow::turnBoard, this, &GameWindow::turnBoard);
    connect(m_settings, &SettingsWindow::turnSecondPlayer, this, &GameWindow::turnSecondPlayer);
    connect(m_settings, &SettingsWindow::autoQueen, this, [this]() {
        m_settingsParams.checkAutoQueen ^= true;
        m_board->setAutoQueen(m_settingsParams.checkAutoQueen);
    });
    connect(m_settings, &SettingsWindow::autoRotate, this, [this]() { m_settingsParams.checkAutoRotate ^= true; });
    connect(m_settings, &SettingsWindow::premove, this, [this]() {
        m_settingsParams.checkPremove ^= true;
        m_board->setPremove(m_settingsParams.checkPremove);
    });
    connect(m_settings, &SettingsWindow::noticeTime, this, [this]() { m_settingsParams.checkNoticeTime ^= true; });
    connect(m_settings, &SettingsWindow::exit, this, [this]() { m_settings->hide(); });
    connect(m_settings, &SettingsWindow::exitGame, this, [this]() {
        m_settings->hide();
        m_endGame->hide();
        emit exitGame();
    });

    connect(m_endGame, &EndGameWindow::newGameSignal, this, [this]() {
        this->startGameInner();
        m_endGame->hide();
    });
    connect(m_endGame, &EndGameWindow::rematchSignal, this, [this]() {
        this->startGameInner();
        this->turnBoard();
        m_endGame->hide();
    });
    connect(m_endGame, &EndGameWindow::blockUserSignal, this, [this]() {
        m_endGame->hide();
        // TODO
    });
    connect(m_endGame, &EndGameWindow::gameReviewSignal, this, [this]() {
        m_endGame->hide();
        // TODO
    });
    connect(m_endGame, &EndGameWindow::exitSignal, this, [this]() { m_endGame->hide(); });

    connect(m_timer, &QTimer::timeout, this, [this]() {
        this->setTime(m_startMove.secsTo(QDateTime::currentDateTime()), m_board->getColorMove());
    });
}

void GameWindow::startGame(StartParams &startParams, ChessBoardParams &boardParams)
{
    m_startParams = startParams;
    m_boardParams = boardParams;

    m_nicknames = {"name1", "name2"};
    m_ratings = {0, 0};
    m_pixmapPlayers = std::pair{QPixmap(QString(GENERAL_PATH) + "player.png"), QPixmap(QString(GENERAL_PATH) + "player.png")};
    bool mainPlayerWhite = true;

    PlayerParams playerParams;
    playerParams.nicknames = m_nicknames;
    playerParams.icons = m_pixmapPlayers;
    playerParams.mainPlayerWhite = mainPlayerWhite;
    playerParams.ratings = m_ratings;

    m_settings->setParams(m_settingsParams, m_startParams.gameType, m_startParams.timeChessType);
    m_endGame->setParams(playerParams, m_startParams.timeChessType);

    this->hideAll();

    if (m_startParams.timeChessType == TypeTimeChess::NO_TIME) {
        m_timePlayers.first->hide();
        m_timePlayers.second->hide();
    } else {
        m_timePlayers.first->show();
        m_timePlayers.second->show();

        m_timePlayers.first->setStyleSheet("background-color: white; color: black; font-size:24px; padding:10px;");
        m_timePlayers.second->setStyleSheet("background-color: black; color: white; font-size:24px; padding:10px;");
    }

    m_iconPlayers.first->setPixmap(m_pixmapPlayers.first);
    m_iconPlayers.second->setPixmap(m_pixmapPlayers.second);

    if (m_ratings.first != 0) {
        m_infoPlayers.first->setText(m_nicknames.first + QString::number(m_ratings.first));
        m_infoPlayers.second->setText(m_nicknames.second + QString::number(m_ratings.second));
    } else {
        m_infoPlayers.first->setText(m_nicknames.first);
        m_infoPlayers.second->setText(m_nicknames.second);
    }

    if (m_startParams.gameType == TypeGame::ONLINE)
        playerParams.ratings = {1000, 1000};
    else
        playerParams.ratings = {0, 0};

    if (!mainPlayerWhite)
        this->turnBoard();

    startGameInner();
}

SettingsParams &GameWindow::getSettingsParams()
{
    return m_settingsParams;
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (m_endGame->isVisible())
        this->showEndGameWindow();

    if (m_settings->isVisible())
        this->showSettingWindow();
}

void GameWindow::showSettingWindow()
{
    const qint16 width = m_board->width() - MINIMUM_PIECE_SIZE;
    const qint16 height = m_board->height();
    m_settings->setGeometry(width / 5 + MINIMUM_PIECE_SIZE, height / 5, width * 3 / 5, height * 3 / 5);
}

void GameWindow::showEndGameWindow()
{
    const qint16 width = m_board->width() - MINIMUM_PIECE_SIZE;
    const qint16 height = m_board->height();
    m_endGame->setGeometry(width / 6 + MINIMUM_PIECE_SIZE, height / 6, width * 2 / 3, height * 2 / 3);
}

void GameWindow::startGameInner()
{
    if (m_startParams.timeChessType != TypeTimeChess::NO_TIME)
        this->resetTime();

    m_board->setBlockBoard(false);

    if (m_startParams.chessType == TypeChess::STANDART)
        m_board->fillChessBoard(false);
    else if (m_startParams.chessType == TypeChess::_960)
        m_board->fillChessBoard(true);
    else
        m_board->fillUserChessBoard(m_boardParams);

    if (m_connection) {
        disconnect(m_upConnectBut);
        disconnect(m_downConnectBut);
    }

    m_board->updateBoard();

    m_upBut->setText("Draw");
    m_downBut->setText("Resign");

    m_connection = true;
    m_upConnectBut = connect(m_upBut, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Draw", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(ResultGame::STALE_MATE);
    });
    m_downConnectBut = connect(m_downBut, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Resign", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(m_board->getColorMove() ? ResultGame::WIN_BLACK : ResultGame::WIN_WHITE);
    });
}

void GameWindow::endGame(ResultGame result)
{
    if (m_startParams.timeChessType != TypeTimeChess::NO_TIME)
        m_timer->stop();

    m_settings->setExitButton(true);
    m_board->setBlockBoard(true);

    m_endGame->setResult(result);
    m_endGame->raise();
    m_endGame->show();

    this->showEndGameWindow();
    
    m_upBut->setText("New Game");
    m_downBut->setText("Rematch");
    
    disconnect(m_upConnectBut);
    disconnect(m_downConnectBut);
    
    m_upConnectBut = connect(m_upBut, &QPushButton::clicked, this, [this]() {
        this->startGameInner();
    });
    m_downConnectBut = connect(m_downBut, &QPushButton::clicked, this, [this]() {
        this->startGameInner();
        this->turnBoard();
    });
}

void GameWindow::newGame() {}

void GameWindow::rematch() {}

void GameWindow::hideAll()
{
    if (m_settingsParams.hideAll) {
        m_players.first->hide();
        m_players.second->hide();
        m_upBut->hide();
        m_downBut->hide();
        m_leftChessHistory->hide();
        m_rightChessHistory->hide();
    } else {
        m_players.first->show();
        m_players.second->show();
        m_upBut->show();
        m_downBut->show();
        m_leftChessHistory->show();
        m_rightChessHistory->show();
    }
}

void GameWindow::turnBoard()
{
    if (m_board->getTurnSecondPlayer()) {
        this->turnPlayerLayout(m_iconPlayers.first, m_infoPlayers.first, m_timePlayers.first,
                               m_pixmapPlayers.first, m_board->getTurnBoard());
        this->turnPlayerLayout(m_iconPlayers.second, m_infoPlayers.second, m_timePlayers.second,
                               m_pixmapPlayers.second, !m_board->getTurnBoard());
    }

    m_board->turnBoard();

    SomeConstans::swapWidgetVBox(m_sideLayout, m_players.first, m_players.second);
    SomeConstans::swapWidgetVBox(m_playersLayout.first, m_iconPlayers.first, m_timePlayers.first);
    SomeConstans::swapWidgetVBox(m_playersLayout.second, m_iconPlayers.second, m_timePlayers.second);

    m_playersLayout.first->swapOrientation();
    m_playersLayout.second->swapOrientation();
}

void GameWindow::turnSecondPlayer()
{
    m_board->turnSecondPlayer();

    if (m_board->getTurnBoard())
        this->turnPlayerLayout(m_iconPlayers.first, m_infoPlayers.first, m_timePlayers.first,
                               m_pixmapPlayers.first, !m_board->getTurnSecondPlayer());
    else
        this->turnPlayerLayout(m_iconPlayers.second, m_infoPlayers.second, m_timePlayers.second,
                               m_pixmapPlayers.second, !m_board->getTurnSecondPlayer());
}

void GameWindow::turnPlayerLayout(QLabel *iconPlayer, RotatableLabel *infoPlayer,
                                  RotatableLabel *timePlayer, QPixmap pixmap, bool up)
{
    if (up) {
        iconPlayer->setPixmap(pixmap);
    } else {
        QTransform transform;
        transform.rotate(180);
        iconPlayer->setPixmap(pixmap.transformed(transform));
    }

    infoPlayer->swapRotatable();
    timePlayer->swapRotatable();
}

void GameWindow::setTime(qint32 seconds, bool white)
{
    qint32 resultTime;
    if (white)
        resultTime = m_time.first - seconds;
    else
        resultTime = m_time.second - seconds;

    if (m_settingsParams.checkNoticeTime &&
            ((m_startParams.mainTime >= 60 * MIN && resultTime <= 5 * MIN) ||
             (m_startParams.mainTime >= 10 * MIN && resultTime <= MIN) ||
             (m_startParams.mainTime >= 3 * MIN && resultTime <= 30) ||
             resultTime <= 10)) {
        if (white)
            m_timePlayers.first->setStyleSheet(m_smallTimeNoticeStyle);
        else
            m_timePlayers.second->setStyleSheet(m_smallTimeNoticeStyle);
    } else {
        if (white)
            m_timePlayers.first->setStyleSheet("background-color: white; color: black; font-size:24px; padding:10px;");
        else
            m_timePlayers.second->setStyleSheet("background-color: black; color: white; font-size:24px; padding:10px;");
    }

    if (resultTime == 0)
        this->endGame(white ? ResultGame::WIN_BLACK : ResultGame::WIN_WHITE);

    const qint8 s = resultTime % 60;
    const qint8 m = resultTime / 60 % 60;
    const qint8 h = resultTime / 60 / 60;

    QString time;
    if (h > 0)
        time = QString::asprintf("%2d:%02d:%02d", h, m, s);
    else
        time = QString::asprintf("%2d:%02d", m, s);

    if (white)
        m_timePlayers.first->setText(time);
    else
        m_timePlayers.second->setText(time);
}

void GameWindow::resetTime()
{
    m_time = {m_startParams.mainTime, m_startParams.mainTime};
    this->setTime(0, true);
    this->setTime(0, false);
}
