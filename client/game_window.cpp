#include "game_window.h"

GameWindow::GameWindow(GameParams &params, QWidget *parent)
    : QWidget{parent}
    , _params{params}
{
    _nicknames = {"name1", "name2"};

    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(pathGeneral + "avatar.png"));

    QPushButton *settings = new QPushButton();
    settings->setFixedSize(35, 35);
    settings->setIcon(QIcon(pathGeneral + "settings.png"));
    settings->setIconSize(QSize(37, 37));
    settings->setStyleSheet("QPushButton { padding: 0px; }");

    QPushButton *draw = new QPushButton();
    draw->setFixedSize(35, 35);
    draw->setIcon(QIcon(pathGeneral + "draw.png"));
    draw->setIconSize(QSize(30, 30));
    draw->setStyleSheet("QPushButton { padding: 0px; }");

    QPushButton *resign = new QPushButton();
    resign->setFixedSize(35, 35);
    resign->setIcon(QIcon(pathGeneral + "resign.png"));
    resign->setIconSize(QSize(31, 31));
    resign->setStyleSheet("QPushButton { padding: 0px; }");

    QHBoxLayout *playingInfo = new QHBoxLayout();
    playingInfo->addWidget(draw);
    playingInfo->addWidget(resign);
    playingInfo->addWidget(settings);

    m_board = new ChessBoard();

    BoardLayout *mainLayout = new BoardLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addLayout(playingInfo);

    this->setLayout(mainLayout);

    EndGameWindow::PlayerParams playerParams;
    playerParams.nicknames = _nicknames;
    playerParams.icons = {QPixmap(pathGeneral + "player.png"), QPixmap(pathGeneral + "player.png")};
    playerParams.mainPlayerWhite = true;
    playerParams.type = _params.chessType;

    if (params.gameType == TypeGame::ONLINE)
        playerParams.ratings = {1000, 1000};
    else
        playerParams.ratings = {0, 0};

    m_endGame = new EndGameWindow(playerParams, this);
    m_endGame->hide();

    connect(m_board, ChessBoard::endGame, this, [this](ResultGame result) {
        m_board->setEnabled(false);
        m_endGame->setResult(result);
        showEndGameWindow();
    });

    connect(m_endGame, EndGameWindow::exitSignal, this, [this]() { m_endGame->hide(); });

    connect(settings, &QPushButton::clicked, this, &GameWindow::showEndGameWindow);
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
