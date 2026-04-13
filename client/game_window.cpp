#include "game_window.h"

#include "src/board_layout.h"

#include <QMessageBox>

GameWindow::GameWindow(QWidget *parent)
    : QWidget{parent}
{
    m_board = new ChessBoard();
    m_settings = new SettingsWindow(this);
    m_endGame = new EndGameWindow(this);
    m_timer = new QTimer(this);
}

void GameWindow::startGame(GameParams &params)
{
    m_StartParams = params.startParams;
    m_SettingsParams = params.settingsParams;

    m_nicknames = {"name1", "name2"};
    m_ratings = {0, 0};
    m_pixmapPlayers = std::pair{QPixmap(pathGeneral + "player.png"), QPixmap(pathGeneral + "player.png")};
    bool mainPlayerWhite = true;

    PlayerParams playerParams;
    playerParams.nicknames = m_nicknames;
    playerParams.icons = m_pixmapPlayers;
    playerParams.mainPlayerWhite = mainPlayerWhite;
    playerParams.type = m_StartParams.timeChessType;
    playerParams.ratings = m_ratings;

    m_settings->setParams(m_SettingsParams, m_StartParams.gameType);
    m_endGame->setParams(playerParams);

    m_board->setAutoQueen(m_SettingsParams.checkAutoQueen);
    m_board->setPremove(m_SettingsParams.checkPremove);
    m_settings->hide();
    m_endGame->hide();

    QPushButton *leftChessHistory = new QPushButton();
    QPushButton *rightChessHistory = new QPushButton();
    QPushButton *settings = new QPushButton();

    leftChessHistory->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);
    rightChessHistory->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);
    settings->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);

    leftChessHistory->setIcon(QIcon(pathGeneral + "leftArrow.png"));
    rightChessHistory->setIcon(QIcon(pathGeneral + "rightArrow.png"));
    settings->setIcon(QIcon(pathGeneral + "settings.png"));

    leftChessHistory->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));
    rightChessHistory->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));
    settings->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));

    leftChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    rightChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    settings->setStyleSheet("QPushButton { padding: 0px; }");

    m_upButton = new QPushButton();
    m_downButton = new QPushButton();

    m_iconPlayers = std::pair{new QLabel(), new QLabel()};
    m_infoPlayers = std::pair{new RotatableLabel(), new RotatableLabel()};
    m_timePlayers = std::pair{new RotatableLabel(), new RotatableLabel()};

    m_iconPlayers.first->setPixmap(m_pixmapPlayers.first);
    m_iconPlayers.second->setPixmap(m_pixmapPlayers.second);
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

    m_timePlayers.first->setStyleSheet("background-color: white; color: black; font-size:24px; padding:10px;");
    m_timePlayers.second->setStyleSheet("background-color: black; color: white; font-size:24px; padding:10px;");

    if (m_ratings.first != 0) {
        m_infoPlayers.first->setText(m_nicknames.first + QString::number(m_ratings.first));
        m_infoPlayers.second->setText(m_nicknames.second + QString::number(m_ratings.second));
    } else {
        m_infoPlayers.first->setText(m_nicknames.first);
        m_infoPlayers.second->setText(m_nicknames.second);
    }

    QHBoxLayout *helperLayout = new QHBoxLayout();

    helperLayout->addWidget(leftChessHistory);
    helperLayout->addWidget(rightChessHistory);
    helperLayout->addWidget(settings);

    m_players = std::pair{new QWidget(), new QWidget()};
    m_playersLayout = std::pair{new PlayerInfoLayout3(), new PlayerInfoLayout3()};

    m_players.first->setLayout(m_playersLayout.first);
    m_players.second->setLayout(m_playersLayout.second);

    m_playersLayout.first->addWidget(m_timePlayers.first);
    m_playersLayout.first->addWidget(m_infoPlayers.first);
    m_playersLayout.first->addWidget(m_iconPlayers.first);
    m_playersLayout.second->addWidget(m_iconPlayers.second);
    m_playersLayout.second->addWidget(m_infoPlayers.second);
    m_playersLayout.second->addWidget(m_timePlayers.second);
    m_playersLayout.first->swapOrientation();

    m_sideLayout = new QVBoxLayout();

    m_sideLayout->addWidget(m_players.second, 2);
    m_sideLayout->addWidget(m_upButton, 1);
    m_sideLayout->addLayout(helperLayout, 1);
    m_sideLayout->addWidget(m_downButton, 1);
    m_sideLayout->addWidget(m_players.first, 2);

    BoardLayout *mainLayout = new BoardLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(m_sideLayout);

    this->setLayout(mainLayout);

    if (m_StartParams.gameType == TypeGame::ONLINE)
        playerParams.ratings = {1000, 1000};
    else
        playerParams.ratings = {0, 0};

    if (!mainPlayerWhite)
        this->turnBoard();

    connect(m_board, &ChessBoard::didMove, this, [this]() {
        if (!m_timer->isActive()) {
            m_startMove = QDateTime::currentDateTime();
            m_timer->start(TICK);
        }

        if (m_SettingsParams.checkAutoRotate)
            this->turnBoard();

        bool white = m_board->getColorMove();
        if (white) {
            m_time.first += m_StartParams.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.first > MAX_TIME_SECONDS)
                m_time.first = MAX_TIME_SECONDS;
        } else {
            m_time.second += m_StartParams.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.second > MAX_TIME_SECONDS)
                m_time.second = MAX_TIME_SECONDS;
        }
        this->setTime(0, white);
        m_startMove = QDateTime::currentDateTime();
    });
    connect(m_board, &ChessBoard::endGame, this, [this](ResultGame result) {
        this->endGame(result);
    });
    connect(leftChessHistory, &QPushButton::clicked, this, [this]() {
        m_board->historyBack();
    });
    connect(rightChessHistory, &QPushButton::clicked, this, [this]() {
        m_board->historyForward();
    });
    connect(settings, &QPushButton::clicked, this, [this]() {
        if (!m_settings->isVisible()) {
            m_settings->raise();
            m_settings->show();
            this->showSettingWindow();
        } else {
            m_settings->hide();
        }
    });
    connect(m_settings, &SettingsWindow::turnBoard, this, &GameWindow::turnBoard);
    connect(m_settings, &SettingsWindow::turnSecondPlayer, this, &GameWindow::turnSecondPlayer);
    connect(m_settings, &SettingsWindow::autoQueen, this, [this]() {
        m_SettingsParams.checkAutoQueen ^= true;
        m_board->setAutoQueen(m_SettingsParams.checkAutoQueen);
    });
    connect(m_settings, &SettingsWindow::autoRotate, this, [this]() {
        m_SettingsParams.checkAutoRotate ^= true;
    });
    connect(m_settings, &SettingsWindow::premove, this, [this]() {
        m_SettingsParams.checkPremove ^= true;
        m_board->setPremove(m_SettingsParams.checkPremove);
    });
    connect(m_settings, &SettingsWindow::noticeTime, this, [this]() {
        m_SettingsParams.checkNoticeTime ^= true;
    });
    connect(m_settings, &SettingsWindow::exitSignal, this, [this]() {
        m_settings->hide();
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
    connect(m_endGame, &EndGameWindow::exitSignal, this, [this]() {
        m_endGame->hide();
    });
    connect(m_timer, &QTimer::timeout, this, [this]() {
        this->setTime(m_startMove.secsTo(QDateTime::currentDateTime()), m_board->getColorMove());
    });

    startGameInner(true);
}

void GameWindow::showSettingWindow()
{
    uint width = m_board->width() - MINIMUM_PIECE_SIZE;
    uint height = m_board->height();
    m_settings->setGeometry(width / 4 + MINIMUM_PIECE_SIZE, height / 4, width / 2, height / 2);
}

void GameWindow::showEndGameWindow()
{
    uint width = m_board->width() - MINIMUM_PIECE_SIZE;
    uint height = m_board->height();
    m_endGame->setGeometry(width / 6 + MINIMUM_PIECE_SIZE, height / 6, width / 3 * 2, height / 3 * 2);
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    if (m_endGame->isVisible())
        this->showEndGameWindow();

    if (m_settings->isVisible())
        this->showSettingWindow();

    QWidget::resizeEvent(event);
}

void GameWindow::startGameInner(bool first)
{
    this->resetTime();

    m_board->setBlockBoard(false);
    if (m_StartParams.chessType == TypeChess::STANDART)
        m_board->fillStandartChessBoard();
    else if (m_StartParams.chessType == TypeChess::STANDART960)
        m_board->fillStandart960ChessBoard();
    else
        m_board->fillUserChessBoard(m_StartParams.chessFields, m_StartParams.chessType == TypeChess::USER ? false : true,
                                    m_StartParams.whiteMove, m_StartParams.castling);

    m_upButton->setText("Draw");
    m_downButton->setText("Resign");
    if (!first) {
        disconnect(m_upButtonCon);
        disconnect(m_downButtonCon);
        m_board->resetBoard();
    }

    m_upButtonCon = connect(m_upButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Draw", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(ResultGame::STALE_MATE);
    });
    m_downButtonCon = connect(m_downButton, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Resign", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(m_board->getColorMove() ? ResultGame::WIN_BLACK : ResultGame::WIN_WHITE);
    });
}

void GameWindow::endGame(ResultGame result)
{
    if (m_timer->isActive())
        m_timer->stop();

    m_board->setBlockBoard(true);

    m_endGame->setResult(result);
    m_endGame->raise();
    m_endGame->show();
    this->showEndGameWindow();

    m_upButton->setText("New Game");
    m_downButton->setText("Rematch");
    disconnect(m_upButtonCon);
    disconnect(m_downButtonCon);

    m_upButtonCon = connect(m_upButton, &QPushButton::clicked, this, [this]() {
        this->startGameInner();
    });
    m_downButtonCon = connect(m_downButton, &QPushButton::clicked, this, [this]() {
        this->startGameInner();
        this->turnBoard();
    });
}

void GameWindow::newGame() {}

void GameWindow::rematch() {}

void GameWindow::turnBoard()
{
    if (m_board->getTurnSecondPlayer()) {
        this->turnPlayerLayout(m_iconPlayers.first, m_infoPlayers.first, m_timePlayers.first,
                               m_pixmapPlayers.first, m_board->getTurnBoard());
        this->turnPlayerLayout(m_iconPlayers.second, m_infoPlayers.second, m_timePlayers.second,
                               m_pixmapPlayers.second, !m_board->getTurnBoard());
    }

    m_board->turnBoard();

    this->turnWidget(m_sideLayout, m_players.first, m_players.second);
    this->turnWidget(m_playersLayout.first, m_iconPlayers.first, m_timePlayers.first);
    this->turnWidget(m_playersLayout.second, m_iconPlayers.second, m_timePlayers.second);

    m_playersLayout.first->swapOrientation();
    m_playersLayout.second->swapOrientation();
}

void GameWindow::turnWidget(QVBoxLayout *layout, QWidget *widget1, QWidget *widget2)
{
    qint8 id1 = layout->indexOf(widget1);
    qint8 id2 = layout->indexOf(widget2);

    layout->removeWidget(widget1);
    layout->removeWidget(widget2);

    if (id1 < id2) {
        layout->insertWidget(id1, widget2);
        layout->insertWidget(id2, widget1);
    } else {
        layout->insertWidget(id2, widget1);
        layout->insertWidget(id1, widget2);
    }
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

    if (resultTime == 0)
        this->endGame(white ? ResultGame::WIN_BLACK : ResultGame::WIN_WHITE);

    qint8 s = resultTime % 60;
    qint8 m = resultTime / 60 % 60;
    qint8 h = resultTime / 60 / 60;

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
    m_time = {m_StartParams.mainTime, m_StartParams.mainTime};
    this->setTime(0, true);
    this->setTime(0, false);
}
