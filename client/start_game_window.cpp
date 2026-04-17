#include "start_game_window.h"

#include <algorithm>

StartGameWindow::StartGameWindow(StartParams &params, QWidget *parent)
    : QWidget{parent}, m_params{params}
{
    m_saveIdTime = {SIDE_SIZE, SIDE_SIZE};

    m_mainLayout = new QVBoxLayout();
    m_stackedTime = new QStackedWidget();
    m_buttonLayout = new QHBoxLayout();

    m_gameTypeButs = new ButtonComplex();
    m_chessTypeButs = new ButtonComplex();
    m_timeChessTypeButs = new ButtonComplex();
    m_timeChessButs = new ButtonComplex();
    m_boardSetupBut = new QPushButton("Board setup");
    m_startGameBut = new QPushButton("Start Game");
    m_timeChessSpins = new TimeChess();
    m_errorLabel = new QLabel();

    m_constans = &SomeConstans::getInstance();
    m_gameTypeButs->setButtons(m_constans->getTypeGameStr());
    m_chessTypeButs->setButtons(m_constans->getTypeChessStr());
    m_timeChessTypeButs->setButtons(m_constans->getTypeTimeChessStr());

    m_errorLabel->setStyleSheet("color: red; font-size: 18px;");
    m_boardSetupBut->adjustSize();
    m_boardSetupBut->setFixedSize(m_boardSetupBut->size());
    m_startGameBut->setFixedHeight(FIXED_SIZE_BUTTON);

    m_stackedTime->addWidget(m_timeChessButs);
    m_stackedTime->addWidget(m_timeChessSpins);

    m_buttonLayout->addStretch(7);
    m_buttonLayout->addWidget(m_boardSetupBut);
    m_buttonLayout->addStretch(1);

    m_mainLayout->setSpacing(0);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_gameTypeButs);
    m_mainLayout->addWidget(m_chessTypeButs);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_timeChessTypeButs);
    m_mainLayout->addWidget(m_stackedTime);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_errorLabel);
    m_mainLayout->addWidget(m_startGameBut);

    m_mainLayout->setAlignment(m_stackedTime, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_errorLabel, Qt::AlignCenter);

    if (m_params.gameType != TypeGame::UNDEFINED)
        m_gameTypeButs->useButton(static_cast<size_t>(m_params.gameType));

    if (m_params.chessType != TypeChess::UNDEFINED)
        m_chessTypeButs->useButton(static_cast<size_t>(m_params.chessType));

    if (m_params.timeChessType != TypeTimeChess::UNDEFINED) {
        m_timeChessTypeButs->useButton(static_cast<size_t>(m_params.timeChessType));

        if (m_params.timeChessType != TypeTimeChess::NO_TIME) {
            if (m_params.timeChessType == TypeTimeChess::OTHER) {
                m_stackedTime->setCurrentWidget(m_timeChessSpins);
            } else {
                m_stackedTime->setCurrentWidget(m_timeChessButs);

                size_t id = static_cast<size_t>(m_params.timeChessType);
                m_timeChessButs->setButtons(m_constans->getTimeChessStr()[id]);

                if (m_params.mainTime != 0) {
                    const auto &vec = m_constans->getTimeChessValue()[id];
                    m_saveIdTime = {id,
                                    std::distance(vec.begin(),
                                                  std::find(vec.begin(), vec.end(), std::pair{m_params.mainTime, m_params.minorTime}))
                                   };
                    m_timeChessButs->useButton(m_saveIdTime.second);
                }
            }
        }
    }

    m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);

    this->lookBoardSetupBut();

    connect(m_gameTypeButs, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();
        m_params.gameType = static_cast<TypeGame>(id);
    });
    connect(m_chessTypeButs, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();
        m_params.chessType = static_cast<TypeChess>(id);
        this->lookBoardSetupBut();
    });
    connect(m_timeChessTypeButs, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();
        m_params.timeChessType = static_cast<TypeTimeChess>(id);

        if (m_params.timeChessType == TypeTimeChess::OTHER) {
            m_stackedTime->setCurrentWidget(m_timeChessSpins);
            m_stackedTime->show();

            const auto &time = m_timeChessSpins->getTime();
            m_params.mainTime = time.first;
            m_params.minorTime = time.second;
        } else if (m_params.timeChessType == TypeTimeChess::NO_TIME) {
            m_stackedTime->hide();

            m_params.mainTime = 0;
            m_params.minorTime = 0;
        } else {
            m_stackedTime->setCurrentWidget(m_timeChessButs);
            m_stackedTime->show();

            m_timeChessButs->setButtons(m_constans->getTimeChessStr()[id]);
            if (id == m_saveIdTime.first) {
                m_timeChessButs->useButton(m_saveIdTime.second);
                const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][m_saveIdTime.second];
                m_params.mainTime = time.first;
                m_params.minorTime = time.second;
            }
        }
    });
    connect(m_boardSetupBut, &QPushButton::clicked, this, &StartGameWindow::boardSetup);
    connect(m_timeChessButs, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();

        m_saveIdTime = {static_cast<qint8>(m_params.timeChessType), id};
        const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][id];
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
        m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);
    });
    connect(m_timeChessSpins, &TimeChess::changeTimeSignal, this, [this]() {
        m_errorLabel->clear();

        m_saveIdTime = {SIDE_SIZE, SIDE_SIZE};
        const auto &time = m_timeChessSpins->getTime();
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
    });
    connect(m_startGameBut, &QPushButton::clicked, this, [this]() {
        if (m_params.gameType == TypeGame::UNDEFINED) {
            m_errorLabel->setText("Game type not selected");
            return;
        }
        if (m_params.chessType == TypeChess::UNDEFINED) {
            m_errorLabel->setText("Chess type not selected");
            return;
        }
        if (m_params.timeChessType == TypeTimeChess::UNDEFINED) {
            m_errorLabel->setText("Сhess time type not selected");
            return;
        }
        if (m_params.timeChessType != TypeTimeChess::NO_TIME && m_params.mainTime == 0) {
            m_errorLabel->setText("Time not selected");
            return;
        }

        m_errorLabel->clear();
        emit startGame();
    });

    this->setLayout(m_mainLayout);
}

void StartGameWindow::hideAllWidget()
{
    m_gameTypeButs->hide();
    m_chessTypeButs->hide();
    m_boardSetupBut->hide();
    m_timeChessTypeButs->hide();
    m_stackedTime->hide();
    m_errorLabel->hide();
    m_startGameBut->hide();
}

void StartGameWindow::showAllWidget()
{
    m_gameTypeButs->show();
    m_chessTypeButs->show();
    m_timeChessTypeButs->show();
    m_errorLabel->show();
    m_startGameBut->show();

    if (m_params.chessType == TypeChess::USER)
        m_boardSetupBut->show();
    if (m_params.timeChessType != TypeTimeChess::NO_TIME)
        m_stackedTime->show();
}

StartParams &StartGameWindow::getStartParams()
{
    return m_params;
}

void StartGameWindow::lookBoardSetupBut()
{
    if (m_params.chessType == TypeChess::USER)
        m_boardSetupBut->show();
    else
        m_boardSetupBut->hide();
}
