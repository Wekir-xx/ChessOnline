#include "end_game_window.h"

#include <QFont>

EndGameWindow::EndGameWindow(QWidget *parent)
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

    m_whiteTurn = true;

    m_mainLayout = new QVBoxLayout();
    m_playerInfo = new QHBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_resultLayout = new QHBoxLayout();
    m_buttonsLayout = new QHBoxLayout();
    m_whitePlayerLayout = new PlayerInfoLayout2();
    m_blackPlayerLayout = new PlayerInfoLayout2();
    m_whitePlayer = new QWidget();
    m_blackPlayer = new QWidget();

    m_exit = new QPushButton();
    m_gameReview = new QPushButton("Game Review");
    m_newGame = new QPushButton("New Game");
    m_rematch = new QPushButton("Rematch");
    m_blockUser = new QPushButton("Block User");

    m_playerWhiteInfo = new QLabel();
    m_playerBlackInfo = new QLabel();
    m_newRatingsWhite = new QLabel();
    m_newRatingsBlack = new QLabel();
    m_result = new QLabel();
    m_iconPlayerWhite = new QLabel();
    m_iconPlayerBlack = new QLabel();
    m_iconGame = new QLabel();

    QFont font = m_result->font();
    font.setPointSize(16);
    font.setBold(true);
    m_result->setFont(font);

    m_exit->setIcon(QIcon(pathGeneral + "exit.png"));
    m_exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exit->setIconSize(m_exit->size());
    m_gameReview->setObjectName("review");

    m_iconPlayerWhite->setScaledContents(true);
    m_iconPlayerBlack->setScaledContents(true);
    m_iconGame->setScaledContents(true);

    m_gameReview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_newGame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_rematch->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_blockUser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_iconPlayerWhite->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_iconPlayerBlack->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_iconGame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    m_iconGame->setFixedSize(FIXED_SIZE_TYPE_GAME, FIXED_SIZE_TYPE_GAME);
    m_playerWhiteInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_playerBlackInfo->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_newRatingsWhite->setFixedHeight(FIXED_SIZE_TYPE_GAME);
    m_newRatingsBlack->setFixedHeight(FIXED_SIZE_TYPE_GAME);

    m_iconPlayerWhite->setAlignment(Qt::AlignCenter);
    m_iconPlayerBlack->setAlignment(Qt::AlignCenter);
    m_iconGame->setAlignment(Qt::AlignCenter);
    m_playerWhiteInfo->setAlignment(Qt::AlignCenter);
    m_playerBlackInfo->setAlignment(Qt::AlignCenter);
    m_newRatingsWhite->setAlignment(Qt::AlignCenter);
    m_newRatingsBlack->setAlignment(Qt::AlignCenter);
    m_result->setAlignment(Qt::AlignCenter);

    m_whitePlayerLayout->addWidget(m_iconPlayerWhite, 1);
    m_whitePlayerLayout->addWidget(m_playerWhiteInfo, 0);

    m_blackPlayerLayout->addWidget(m_iconPlayerBlack, 1);
    m_blackPlayerLayout->addWidget(m_playerBlackInfo, 0);

    m_whitePlayer->setLayout(m_whitePlayerLayout);
    m_blackPlayer->setLayout(m_blackPlayerLayout);

    m_playerInfo->addWidget(m_whitePlayer, 1);
    m_playerInfo->addWidget(m_blackPlayer, 1);

    m_topLayout->addSpacing(20);
    m_topLayout->addWidget(m_result, 20);
    m_topLayout->addWidget(m_exit, 1);

    m_buttonsLayout->addWidget(m_newGame);
    m_buttonsLayout->addWidget(m_rematch);
    m_buttonsLayout->addWidget(m_blockUser);

    m_resultLayout->addWidget(m_newRatingsWhite);
    m_resultLayout->addWidget(m_iconGame);
    m_resultLayout->addWidget(m_newRatingsBlack);

    m_mainLayout->addLayout(m_topLayout, 1);
    m_mainLayout->addLayout(m_playerInfo, 4);
    m_mainLayout->addSpacing(15);
    m_mainLayout->addLayout(m_resultLayout, 1);
    m_mainLayout->addWidget(m_gameReview, 1);
    m_mainLayout->addLayout(m_buttonsLayout, 1);

    connect(m_exit, &QPushButton::clicked, this, &EndGameWindow::exitSignal);
    connect(m_newGame, &QPushButton::clicked, this, &EndGameWindow::newGameSignal);
    connect(m_rematch, &QPushButton::clicked, this, &EndGameWindow::rematchSignal);
    connect(m_gameReview, &QPushButton::clicked, this, &EndGameWindow::gameReviewSignal);
    connect(m_blockUser, &QPushButton::clicked, this, &EndGameWindow::blockUserSignal);

    this->setLayout(m_mainLayout);
}

void EndGameWindow::setParams(PlayerParams params, TypeTimeChess timeChessType)
{
    if (timeChessType == TypeTimeChess::BULLET)
        m_iconGame->setPixmap(QPixmap(pathGeneral + "bullet.png"));
    else if (timeChessType == TypeTimeChess::BLITZ)
        m_iconGame->setPixmap(QPixmap(pathGeneral + "blitz.png"));
    else if (timeChessType == TypeTimeChess::RAPID)
        m_iconGame->setPixmap(QPixmap(pathGeneral + "rapid.png"));
    else if (timeChessType == TypeTimeChess::CLASSIC)
        m_iconGame->setPixmap(QPixmap(pathGeneral + "classic.png"));
    else if (timeChessType == TypeTimeChess::OTHER)
        m_iconGame->setPixmap(QPixmap(pathGeneral + "other.png"));

    m_iconPlayerWhite->setPixmap(params.icons.first);
    m_iconPlayerBlack->setPixmap(params.icons.second);

    QString playerWhiteInfoStr = params.nicknames.first;
    QString playerBlackInfoStr = params.nicknames.second;

    if (params.ratings.first != 0) {
        m_blockUser->show();

        playerWhiteInfoStr += " " + QString::number(params.ratings.first);
        playerBlackInfoStr += " " + QString::number(params.ratings.second);
    } else {
        m_blockUser->hide();
    }

    m_playerWhiteInfo->setText(playerWhiteInfoStr);
    m_playerBlackInfo->setText(playerBlackInfoStr);

    if ((params.mainPlayerWhite && !m_whiteTurn) || (!params.mainPlayerWhite && m_whiteTurn)) {
        m_whiteTurn ^= true;
        SomeConstans::swapWidgetHBox(m_playerInfo, m_whitePlayer, m_blackPlayer);
        SomeConstans::swapWidgetHBox(m_resultLayout, m_newRatingsWhite, m_newRatingsBlack);
    }
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
