#include "game_window.h"

GameWindow::GameWindow(GameParams &params, QWidget *parent)
    : QWidget{parent}
    , m_params{params}
{
    m_nicknames = {"name1", "name2"};
    bool mainPlayerWhite = true;

    EndGameWindow::PlayerParams playerParams;
    playerParams.nicknames = m_nicknames;
    playerParams.icons = {QPixmap(pathGeneral + "player.png"), QPixmap(pathGeneral + "player.png")};
    playerParams.mainPlayerWhite = mainPlayerWhite;
    playerParams.type = m_params.timeChessType;

    m_board = new ChessBoard();
    m_settings = new SettingsWindow(params.settingParams, this);
    m_endGame = new EndGameWindow(playerParams, this);
    m_timer = new QTimer(this);

    m_board->setAutoQueen(m_params.settingParams.checkAutoQueen);
    m_board->setPremove(m_params.settingParams.checkPremove);
    m_settings->hide();
    m_endGame->hide();

    QPushButton *leftChessHistory = new QPushButton();
    leftChessHistory->setFixedSize(35, 35);
    leftChessHistory->setIcon(QIcon(pathGeneral + "leftArrow.png"));
    leftChessHistory->setIconSize(QSize(37, 37));
    leftChessHistory->setStyleSheet("QPushButton { padding: 0px; }");

    QPushButton *rightChessHistory = new QPushButton();
    rightChessHistory->setFixedSize(35, 35);
    rightChessHistory->setIcon(QIcon(pathGeneral + "rightArrow.png"));
    rightChessHistory->setIconSize(QSize(37, 37));
    rightChessHistory->setStyleSheet("QPushButton { padding: 0px; }");

    QPushButton *settings = new QPushButton();
    settings->setFixedSize(35, 35);
    settings->setIcon(QIcon(pathGeneral + "settings.png"));
    settings->setIconSize(QSize(37, 37));
    settings->setStyleSheet("QPushButton { padding: 0px; }");

    m_upButton = new QPushButton();
    m_downButton = new QPushButton();

    m_timeLabel = std::pair{new QLabel(), new QLabel()};
    m_timeLabel.first->setAlignment(Qt::AlignCenter);
    m_timeLabel.second->setAlignment(Qt::AlignCenter);
    m_timeLabel.first->setStyleSheet("background-color: white; color: black; font-size:24px; padding:10px;");
    m_timeLabel.second->setStyleSheet("background-color: black; color: white; font-size:24px; padding:10px;");
    m_timeLabel.first->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    m_timeLabel.second->setFixedHeight(FIXED_SIZE_TYPE_GAME);

    QHBoxLayout *helperLayout = new QHBoxLayout();
    helperLayout->addWidget(leftChessHistory);
    helperLayout->addWidget(rightChessHistory);
    helperLayout->addWidget(settings);

    m_sideLayout = new QVBoxLayout();
    m_sideLayout->addWidget(m_timeLabel.second);
    m_sideLayout->addWidget(m_upButton);
    m_sideLayout->addLayout(helperLayout);
    m_sideLayout->addWidget(m_downButton);
    m_sideLayout->addWidget(m_timeLabel.first);

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

        bool white = !m_board->getColorMove();
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
    connect(leftChessHistory, &QPushButton::clicked, this, [this]() { m_board->historyBack(); });
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
    connect(m_settings, &SettingsWindow::turnBoard, this, [this]() { this->turnBoard(); });
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
    connect(m_endGame, &EndGameWindow::exitSignal, this, [this]() { m_endGame->hide(); });
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
        m_board->fillUserChessBoard(m_params.chessFields, m_params.chessType == TypeChess::USER ? false : true, m_params.castling);

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

    qint8 id1 = m_sideLayout->indexOf(m_timeLabel.first);
    qint8 id2 = m_sideLayout->indexOf(m_timeLabel.second);

    m_sideLayout->removeWidget(m_timeLabel.first);
    m_sideLayout->removeWidget(m_timeLabel.second);

    if (id1 < id2) {
        m_sideLayout->insertWidget(id1, m_timeLabel.second);
        m_sideLayout->insertWidget(id2, m_timeLabel.first);
    } else {
        m_sideLayout->insertWidget(id2, m_timeLabel.first);
        m_sideLayout->insertWidget(id1, m_timeLabel.second);
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
        m_timeLabel.first->setText(time);
    else
        m_timeLabel.second->setText(time);
}

void GameWindow::resetTime()
{
    m_time = {m_params.mainTime, m_params.mainTime};
    this->setTime(0, true);
    this->setTime(0, false);
}
