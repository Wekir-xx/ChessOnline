#include "game_window.h"

GameWindow::GameWindow(GameParams &params, QWidget *parent)
    : QWidget{parent}
    , m_params{params}
{
    m_nicknames = {"name1", "name2"};

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

    m_board = new ChessBoard();
    if (m_params.chessType == TypeChess::STANDART)
        m_board->fillStandartChessBoard();
    else if (m_params.chessType == TypeChess::STANDART960)
        m_board->fillStandart960ChessBoard();
    else
        m_board->fillUserChessBoard(m_params.chessFields, m_params.chessType == TypeChess::USER ? false : true, m_params.castling);

    BoardLayout *mainLayout = new BoardLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(playingInfo);

    this->setLayout(mainLayout);

    EndGameWindow::PlayerParams playerParams;
    playerParams.nicknames = m_nicknames;
    playerParams.icons = {QPixmap(pathGeneral + "player.png"), QPixmap(pathGeneral + "player.png")};
    playerParams.mainPlayerWhite = true;
    playerParams.type = m_params.timeChessType;

    if (params.gameType == TypeGame::ONLINE)
        playerParams.ratings = {1000, 1000};
    else
        playerParams.ratings = {0, 0};

    m_endGame = new EndGameWindow(playerParams, this);
    m_endGame->hide();

    connect(m_board, ChessBoard::endGame, this, [this](ResultGame result) { this->endGame(result); });

    connect(m_endGame, EndGameWindow::exitSignal, this, [this]() { m_endGame->hide(); });

    startGame();
}

void GameWindow::showEndGameWindow()
{
    uint width = m_board->width() - FIXED_SIZE_NUMBERS;
    uint height = m_board->height();
    m_endGame->setGeometry(width / 6 + FIXED_SIZE_NUMBERS, height / 6, width / 3 * 2, height / 3 * 2);

    m_endGame->raise();
    m_endGame->show();
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    if (m_endGame->isVisible())
        showEndGameWindow();

    QWidget::resizeEvent(event);
}

void GameWindow::startGame()
{
    m_upButton->setText("Draw");
    m_downButton->setText("Resign");

    connect(m_upButton, QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Draw", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(ResultGame::STALE_MATE);
    });

    connect(m_downButton, QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Resign", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes)
            this->endGame(m_board->getColorMove() ? ResultGame::WIN_BLACK : ResultGame::WIN_WHITE);
    });
}

void GameWindow::endGame(ResultGame result)
{
    m_upButton->setText("New Game");
    m_downButton->setText("Rematch");

    m_board->setEnabled(false);
    m_endGame->setResult(result);
    showEndGameWindow();

    connect(m_upButton, QPushButton::clicked, this, [this]() { this->newGame(); });

    connect(m_downButton, QPushButton::clicked, this, [this]() { this->rematch(); });
}

void GameWindow::newGame() {}

void GameWindow::rematch() {}
