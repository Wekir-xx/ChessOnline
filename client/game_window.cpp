#include "game_window.h"

GameWindow::GameWindow(GameParams &params, QWidget *parent)
    : QWidget{parent}
    , m_params{params}
{
    m_nicknames = {"name1", "name2"};
    m_ratings = {0, 0};
    bool mainPlayerWhite = true;

    EndGameWindow::PlayerParams playerParams;
    playerParams.nicknames = m_nicknames;
    playerParams.icons = {QPixmap(pathGeneral + "player.png"), QPixmap(pathGeneral + "player.png")};
    playerParams.mainPlayerWhite = mainPlayerWhite;
    playerParams.type = m_params.timeChessType;
    playerParams.ratings = m_ratings;

    m_board = new ChessBoard();
    m_settings = new SettingsWindow(params.settingParams, this);
    m_endGame = new EndGameWindow(playerParams, this);
    m_timer = new QTimer(this);

    m_board->setAutoQueen(m_params.settingParams.checkAutoQueen);
    m_board->setPremove(m_params.settingParams.checkPremove);
    m_settings->hide();
    m_endGame->hide();

    QPushButton *leftChessHistory = new QPushButton();
    QPushButton *rightChessHistory = new QPushButton();
    QPushButton *settings = new QPushButton();

    leftChessHistory->setFixedSize(FIXED_SIZE_MINI_BUTTON, FIXED_SIZE_MINI_BUTTON);
    rightChessHistory->setFixedSize(FIXED_SIZE_MINI_BUTTON, FIXED_SIZE_MINI_BUTTON);
    settings->setFixedSize(FIXED_SIZE_MINI_BUTTON, FIXED_SIZE_MINI_BUTTON);

    leftChessHistory->setIcon(QIcon(pathGeneral + "leftArrow.png"));
    rightChessHistory->setIcon(QIcon(pathGeneral + "rightArrow.png"));
    settings->setIcon(QIcon(pathGeneral + "settings.png"));

    leftChessHistory->setIconSize(QSize(FIXED_SIZE_MINI_BUTTON_ICON, FIXED_SIZE_MINI_BUTTON_ICON));
    rightChessHistory->setIconSize(QSize(FIXED_SIZE_MINI_BUTTON_ICON, FIXED_SIZE_MINI_BUTTON_ICON));
    settings->setIconSize(QSize(FIXED_SIZE_MINI_BUTTON_ICON, FIXED_SIZE_MINI_BUTTON_ICON));

    leftChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    rightChessHistory->setStyleSheet("QPushButton { padding: 0px; }");
    settings->setStyleSheet("QPushButton { padding: 0px; }");

    m_upButton = new QPushButton();
    m_downButton = new QPushButton();

    m_iconPlayer = std::pair{new QLabel(), new QLabel()};
    m_infoPlayer = std::pair{new QLabel(), new QLabel()};
    m_timePlayer = std::pair{new QLabel(), new QLabel()};

    m_iconPlayer.first->setPixmap(playerParams.icons.first);
    m_iconPlayer.second->setPixmap(playerParams.icons.second);
    m_iconPlayer.first->setScaledContents(true);
    m_iconPlayer.second->setScaledContents(true);
    m_iconPlayer.first->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_iconPlayer.second->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_infoPlayer.first->setAlignment(Qt::AlignCenter);
    m_infoPlayer.second->setAlignment(Qt::AlignCenter);
    m_timePlayer.first->setAlignment(Qt::AlignCenter);
    m_timePlayer.second->setAlignment(Qt::AlignCenter);
    m_infoPlayer.first->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_infoPlayer.second->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_timePlayer.first->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    m_timePlayer.second->setFixedHeight(FIXED_SIZE_TYPE_GAME);

    m_timePlayer.first->setStyleSheet("background-color: white; color: black; font-size:24px; padding:10px;");
    m_timePlayer.second->setStyleSheet("background-color: black; color: white; font-size:24px; padding:10px;");

    if (m_ratings.first != 0) {
        m_infoPlayer.first->setText(m_nicknames.first + QString::number(m_ratings.first));
        m_infoPlayer.second->setText(m_nicknames.second + QString::number(m_ratings.second));
    } else {
        m_infoPlayer.first->setText(m_nicknames.first);
        m_infoPlayer.second->setText(m_nicknames.second);
    }

    QHBoxLayout *helperLayout = new QHBoxLayout();

    helperLayout->addWidget(leftChessHistory);
    helperLayout->addWidget(rightChessHistory);
    helperLayout->addWidget(settings);

    m_player = std::pair{new QWidget(), new QWidget()};
    m_playerLayout = std::pair{new QVBoxLayout(), new QVBoxLayout()};

    m_player.first->setLayout(m_playerLayout.first);
    m_player.second->setLayout(m_playerLayout.second);

    m_playerLayout.first->addWidget(m_timePlayer.first);
    m_playerLayout.first->addWidget(m_infoPlayer.first);
    m_playerLayout.first->addWidget(m_iconPlayer.first);
    m_playerLayout.second->addWidget(m_iconPlayer.second);
    m_playerLayout.second->addWidget(m_infoPlayer.second);
    m_playerLayout.second->addWidget(m_timePlayer.second);

    m_sideLayout = new QVBoxLayout();

    m_sideLayout->addWidget(m_player.second);
    m_sideLayout->addWidget(m_upButton);
    m_sideLayout->addLayout(helperLayout);
    m_sideLayout->addWidget(m_downButton);
    m_sideLayout->addWidget(m_player.first);

    BoardLayout *mainLayout = new BoardLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(m_sideLayout);

    this->setLayout(mainLayout);

    if (params.settingParams.gameType == TypeGame::ONLINE)
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

        bool white = m_board->getColorMove();
        if (white) {
            m_time.first += m_params.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.first > MAX_TIME_SECONDS)
                m_time.first = MAX_TIME_SECONDS;
        } else {
            m_time.second += m_params.minorTime - m_startMove.secsTo(QDateTime::currentDateTime());
            if (m_time.second > MAX_TIME_SECONDS)
                m_time.second = MAX_TIME_SECONDS;
        }
        this->setTime(0, white);
        m_startMove = QDateTime::currentDateTime();
    });
    connect(m_board, &ChessBoard::endGame, this, [this](ResultGame result) {
        this->endGame(result);
        m_timer->stop();
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
    connect(m_settings, &SettingsWindow::turnBoard, this, [this]() {
        this->turnBoard();
    });
    connect(m_settings, &SettingsWindow::turnChess, this, [this]() {
        m_board->turnChess();
    });
    connect(m_settings, &SettingsWindow::autoQueen, this, [this]() {
        m_params.settingParams.checkAutoQueen ^= true;
        m_board->setAutoQueen(m_params.settingParams.checkAutoQueen);
    });
    connect(m_settings, &SettingsWindow::premove, this, [this]() {
        m_params.settingParams.checkPremove ^= true;
        m_board->setPremove(m_params.settingParams.checkPremove);
    });
    connect(m_settings, &SettingsWindow::noticeTime, this, [this]() {
        m_params.settingParams.checkNoticeTime ^= true;
    });
    connect(m_settings, &SettingsWindow::exitSignal, this, [this]() {
        m_settings->hide();
    });
    connect(m_endGame, &EndGameWindow::newGameSignal, this, [this]() {
        this->startGame();
        m_endGame->hide();
    });
    connect(m_endGame, &EndGameWindow::rematchSignal, this, [this]() {
        this->startGame();
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

    startGame(true);
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

void GameWindow::startGame(bool first)
{
    this->resetTime();

    m_board->setBlockBoard(false);
    if (m_params.chessType == TypeChess::STANDART)
        m_board->fillStandartChessBoard();
    else if (m_params.chessType == TypeChess::STANDART960)
        m_board->fillStandart960ChessBoard();
    else
        m_board->fillUserChessBoard(m_params.chessFields, m_params.chessType == TypeChess::USER ? false : true,
                                    m_params.whiteMove, m_params.castling);

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
        this->startGame();
    });
    m_downButtonCon = connect(m_downButton, &QPushButton::clicked, this, [this]() {
        this->startGame();
        this->turnBoard();
    });
}

void GameWindow::newGame() {}

void GameWindow::rematch() {}

void GameWindow::turnBoard()
{
    m_board->turnBoard();
    turnWidget(m_sideLayout, m_player.first, m_player.second);
    turnWidget(m_playerLayout.first, m_iconPlayer.first, m_timePlayer.first);
    turnWidget(m_playerLayout.second, m_iconPlayer.second, m_timePlayer.second);
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
        m_timePlayer.first->setText(time);
    else
        m_timePlayer.second->setText(time);
}

void GameWindow::resetTime()
{
    m_time = {m_params.mainTime, m_params.mainTime};
    this->setTime(0, true);
    this->setTime(0, false);
}
