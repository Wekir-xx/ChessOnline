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

    m_board->setAutoQueen(m_params.settingParams.checkAutoQueen);
    m_board->setPremove(m_params.settingParams.checkPremove);
    m_settings->hide();
    m_endGame->hide();

    QPushButton *settings = new QPushButton();
    settings->setFixedSize(35, 35);
    settings->setIcon(QIcon(pathGeneral + "settings.png"));
    settings->setIconSize(QSize(37, 37));
    settings->setStyleSheet("QPushButton { padding: 0px; }");

    m_upButton = new QPushButton();
    m_downButton = new QPushButton();

    QVBoxLayout *playingInfo = new QVBoxLayout();
    playingInfo->addWidget(m_upButton);
    playingInfo->addWidget(settings);
    playingInfo->addWidget(m_downButton);

    BoardLayout *mainLayout = new BoardLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(playingInfo);

    this->setLayout(mainLayout);

    if (params.gameType == TypeGame::ONLINE)
        playerParams.ratings = {1000, 1000};
    else
        playerParams.ratings = {0, 0};

    if (!mainPlayerWhite)
        m_board->turnBoard();

    connect(m_board, &ChessBoard::endGame, this, [this](ResultGame result) {
        this->endGame(result);
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
        m_board->turnBoard();
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
        m_board->turnBoard();
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
    if (m_params.chessType == TypeChess::STANDART)
        m_board->fillStandartChessBoard();
    else if (m_params.chessType == TypeChess::STANDART960)
        m_board->fillStandart960ChessBoard();
    else
        m_board->fillUserChessBoard(m_params.chessFields, m_params.chessType == TypeChess::USER ? false : true, m_params.castling);

    if (!m_board->isEnabled())
        m_board->setEnabled(true);

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
    m_board->setEnabled(false);

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
        m_board->turnBoard();
    });
}

void GameWindow::newGame() {}

void GameWindow::rematch() {}
