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
    QPushButton *review = new QPushButton("Game Review");
    QPushButton *newGame = new QPushButton("New Game");
    QPushButton *rematch = new QPushButton("Rematch");

    exit->setIcon(QIcon(pathGeneral + "exit.png"));
    exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    exit->setIconSize(exit->size());
    review->setObjectName("review");

    QLabel *iconPlayerWhite = new QLabel();
    QLabel *iconPlayerBlack = new QLabel();
    QLabel *iconGame = new QLabel();

    if (params.type == TypeChess::BULLET)
        iconGame->setPixmap(QPixmap(pathGeneral + "bullet.png"));
    else if (params.type == TypeChess::BLITZ)
        iconGame->setPixmap(QPixmap(pathGeneral + "blitz.png"));
    else if (params.type == TypeChess::RAPID)
        iconGame->setPixmap(QPixmap(pathGeneral + "rapid.png"));
    else if (params.type == TypeChess::CLASSIC)
        iconGame->setPixmap(QPixmap(pathGeneral + "classic.png"));
    else if (params.type == TypeChess::OTHER)
        iconGame->setPixmap(QPixmap(pathGeneral + "other.png"));

    iconGame->setFixedSize(FIXED_SIZE_TYPE_GAME, FIXED_SIZE_TYPE_GAME);
    iconPlayerWhite->setPixmap(params.icons.first);
    iconPlayerBlack->setPixmap(params.icons.second);
    iconPlayerWhite->setAlignment(Qt::AlignCenter);
    iconPlayerBlack->setAlignment(Qt::AlignCenter);
    iconGame->setAlignment(Qt::AlignCenter);
    iconPlayerWhite->setScaledContents(true);
    iconPlayerBlack->setScaledContents(true);
    iconGame->setScaledContents(true);

    QString playerWhiteInfoStr = params.nicknames.first;
    QString playerBlackInfoStr = params.nicknames.second;

    if (params.ratings.first != 0) {
        QPushButton *blockUser = new QPushButton("Block User");
        buttonsLayout->addWidget(blockUser);

        playerWhiteInfoStr += " " + QString::number(params.ratings.first);
        playerBlackInfoStr += " " + QString::number(params.ratings.second);
    }

    QLabel *playerWhiteInfo = new QLabel(playerWhiteInfoStr);
    QLabel *playerBlackInfo = new QLabel(playerBlackInfoStr);

    playerWhiteInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    playerBlackInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    playerWhiteInfo->setAlignment(Qt::AlignCenter);
    playerBlackInfo->setAlignment(Qt::AlignCenter);

    whitePlayer->addWidget(iconPlayerWhite);
    whitePlayer->addWidget(playerWhiteInfo);
    blackPlayer->addWidget(iconPlayerBlack);
    blackPlayer->addWidget(playerBlackInfo);

    if (params.mainPlayerWhite) {
        playerInfo->addLayout(whitePlayer);
        playerInfo->addLayout(blackPlayer);
    } else {
        playerInfo->addLayout(blackPlayer);
        playerInfo->addLayout(whitePlayer);
    }

    topLayout->addStretch();
    topLayout->addWidget(exit);
    resultLayout->addWidget(iconGame);
    buttonsLayout->addWidget(newGame);
    buttonsLayout->addWidget(rematch);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(playerInfo);
    mainLayout->addLayout(resultLayout);
    mainLayout->addWidget(review);
    mainLayout->addLayout(buttonsLayout);

    this->setLayout(mainLayout);

    connect(exit, &QPushButton::clicked, this, &EndGameWindow::exitSignal);
}

void EndGameWindow::setResult(ResultGame result, std::pair<qint16, qint16> newRatings) {}
