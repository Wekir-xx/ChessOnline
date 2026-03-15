#include "end_game_window.h"

EndGameWindow::EndGameWindow(PlayerParams params, QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(true);
    this->setStyleSheet("EndGameWindow {"
                        "   background-color: #D1D1D1;"
                        "   border: 2px solid #808080;"
                        "   border-radius: 10px;"
                        "}"
                        "EndGameWindow QLabel {"
                        "   color: #000000;"
                        "}"
                        "EndGameWindow QPushButton {"
                        "   color: #000000;"
                        "   background-color: #e0e0e0;"
                        "   border: 1px solid #a0a0a0;"
                        "   border-radius: 5px;"
                        "   padding: 5px;"
                        "}"
                        "EndGameWindow QPushButton:hover {"
                        "   background-color: #d0d0d0;"
                        "}"
                        "EndGameWindow QPushButton#review {"
                        "   background-color: #30C722;"
                        "}"
                        "EndGameWindow QPushButton#review:hover {"
                        "   background-color: #25A619;"
                        "}");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *playerInfo = new QHBoxLayout();
    PlayerInfoLayout *whitePlayer = new PlayerInfoLayout();
    PlayerInfoLayout *blackPlayer = new PlayerInfoLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *resultLayout = new QHBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QPushButton *exit = new QPushButton();
    QPushButton *gameReview = new QPushButton("Game Review");
    QPushButton *newGame = new QPushButton("New Game");
    QPushButton *rematch = new QPushButton("Rematch");

    exit->setIcon(QIcon(pathGeneral + "exit.png"));
    exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    exit->setIconSize(exit->size());
    gameReview->setObjectName("review");
    gameReview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    newGame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rematch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLabel *iconPlayerWhite = new QLabel();
    QLabel *iconPlayerBlack = new QLabel();
    QLabel *iconGame = new QLabel();

    if (params.type == TypeTimeChess::BULLET)
        iconGame->setPixmap(QPixmap(pathGeneral + "bullet.png"));
    else if (params.type == TypeTimeChess::BLITZ)
        iconGame->setPixmap(QPixmap(pathGeneral + "blitz.png"));
    else if (params.type == TypeTimeChess::RAPID)
        iconGame->setPixmap(QPixmap(pathGeneral + "rapid.png"));
    else if (params.type == TypeTimeChess::CLASSIC)
        iconGame->setPixmap(QPixmap(pathGeneral + "classic.png"));
    else if (params.type == TypeTimeChess::OTHER)
        iconGame->setPixmap(QPixmap(pathGeneral + "other.png"));

    iconPlayerWhite->setPixmap(params.icons.first);
    iconPlayerBlack->setPixmap(params.icons.second);
    iconPlayerWhite->setAlignment(Qt::AlignCenter);
    iconPlayerBlack->setAlignment(Qt::AlignCenter);
    iconGame->setAlignment(Qt::AlignCenter);
    iconPlayerWhite->setScaledContents(true);
    iconPlayerBlack->setScaledContents(true);
    iconGame->setScaledContents(true);
    iconPlayerWhite->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    iconPlayerBlack->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    iconGame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    iconGame->setFixedSize(FIXED_SIZE_TYPE_GAME, FIXED_SIZE_TYPE_GAME);

    QString playerWhiteInfoStr = params.nicknames.first;
    QString playerBlackInfoStr = params.nicknames.second;

    if (params.ratings.first != 0) {
        QPushButton *blockUser = new QPushButton("Block User");
        blockUser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        buttonsLayout->addWidget(blockUser);
        connect(blockUser, &QPushButton::clicked, this, &EndGameWindow::blockUserSignal);

        playerWhiteInfoStr += " " + QString::number(params.ratings.first);
        playerBlackInfoStr += " " + QString::number(params.ratings.second);
    }

    QLabel *playerWhiteInfo = new QLabel(playerWhiteInfoStr);
    QLabel *playerBlackInfo = new QLabel(playerBlackInfoStr);
    m_newRatingsWhite = new QLabel();
    m_newRatingsBlack = new QLabel();
    m_result = new QLabel();

    playerWhiteInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    playerBlackInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_newRatingsWhite->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    m_newRatingsBlack->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    playerWhiteInfo->setAlignment(Qt::AlignCenter);
    playerBlackInfo->setAlignment(Qt::AlignCenter);
    m_newRatingsWhite->setAlignment(Qt::AlignCenter);
    m_newRatingsBlack->setAlignment(Qt::AlignCenter);
    m_result->setAlignment(Qt::AlignCenter);

    whitePlayer->addWidget(iconPlayerWhite, 1);
    whitePlayer->addWidget(playerWhiteInfo, 0);
    blackPlayer->addWidget(iconPlayerBlack, 1);
    blackPlayer->addWidget(playerBlackInfo, 0);

    if (params.mainPlayerWhite) {
        playerInfo->addLayout(whitePlayer, 1);
        playerInfo->addLayout(blackPlayer, 1);

        resultLayout->addWidget(m_newRatingsWhite);
        resultLayout->addWidget(iconGame);
        resultLayout->addWidget(m_newRatingsBlack);
    } else {
        playerInfo->addLayout(blackPlayer, 1);
        playerInfo->addLayout(whitePlayer, 1);

        resultLayout->addWidget(m_newRatingsBlack);
        resultLayout->addWidget(iconGame);
        resultLayout->addWidget(m_newRatingsWhite);
    }

    topLayout->addWidget(m_result, 10);
    topLayout->addWidget(exit, 1);
    buttonsLayout->addWidget(newGame);
    buttonsLayout->addWidget(rematch);
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addLayout(playerInfo, 4);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(resultLayout, 1);
    mainLayout->addWidget(gameReview, 1);
    mainLayout->addLayout(buttonsLayout, 1);

    connect(exit, &QPushButton::clicked, this, &EndGameWindow::exitSignal);
    connect(newGame, &QPushButton::clicked, this, &EndGameWindow::newGameSignal);
    connect(rematch, &QPushButton::clicked, this, &EndGameWindow::rematchSignal);
    connect(gameReview, &QPushButton::clicked, this, &EndGameWindow::gameReviewSignal);

    this->setLayout(mainLayout);
}

void EndGameWindow::setResult(ResultGame result, std::pair<qint16, qint16> newRatings)
{
    if (result == ResultGame::WIN_WHITE)
        m_result->setText("White win!");
    else if (result == ResultGame::WIN_BLACK)
        m_result->setText("Black win!");
    else
        m_result->setText("Stalemate");

    m_newRatingsWhite->setText(QString::number(newRatings.first));
    m_newRatingsBlack->setText(QString::number(newRatings.second));
}
