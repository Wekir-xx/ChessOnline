#include "start_game_window.h"

#include <algorithm>

StartGameWindow::StartGameWindow(StartParams &params, QWidget *parent)
    : QWidget{parent}, m_params{params}
{
    m_saveIdTime = {EMPTY, EMPTY};

    m_mainLayout = new QVBoxLayout();
    m_stackedTime = new QStackedWidget();

    m_gameTypeButtons = new ButtonComplex();
    m_chessTypeButtons = new ButtonComplex();
    m_timeChessTypeButtons = new ButtonComplex();
    m_timeChessButtons = new ButtonComplex();
    m_timeChessSpins = new TimeChess();
    m_errorLabel = new QLabel();
    m_startGame = new QPushButton("Start Game");

    m_constans = &SomeConstans::getInstance();
    m_gameTypeButtons->setButtons(m_constans->getTypeGameStr());
    m_chessTypeButtons->setButtons(m_constans->getTypeChessStr());
    m_timeChessTypeButtons->setButtons(m_constans->getTypeTimeChessStr());

    m_errorLabel->setStyleSheet("color: red;"
                                "font-size: 18px;");
    m_startGame->setFixedHeight(FIXED_SIZE_BUTTON);

    m_stackedTime->addWidget(m_timeChessButtons);
    m_stackedTime->addWidget(m_timeChessSpins);

    m_mainLayout->setSpacing(0);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_gameTypeButtons);
    m_mainLayout->addWidget(m_chessTypeButtons);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_timeChessTypeButtons);
    m_mainLayout->addWidget(m_stackedTime);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_errorLabel);
    m_mainLayout->addWidget(m_startGame);

    m_mainLayout->setAlignment(m_stackedTime, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_errorLabel, Qt::AlignCenter);

    if (m_params.gameType != TypeGame::UNDEFINED)
        m_gameTypeButtons->useButton(static_cast<size_t>(m_params.gameType));

    if (m_params.chessType != TypeChess::UNDEFINED)
        m_chessTypeButtons->useButton(static_cast<size_t>(m_params.chessType));

    if (m_params.timeChessType != TypeTimeChess::UNDEFINED) {
        m_timeChessTypeButtons->useButton(static_cast<size_t>(m_params.timeChessType));

        if (m_params.timeChessType == TypeTimeChess::OTHER) {
            m_stackedTime->setCurrentWidget(m_timeChessSpins);
        } else {
            m_stackedTime->setCurrentWidget(m_timeChessButtons);

            size_t id = static_cast<size_t>(m_params.timeChessType);
            m_timeChessButtons->setButtons(m_constans->getTimeChessStr()[id]);

            if (m_params.mainTime != 0) {
                const auto &vec = m_constans->getTimeChessValue()[id];
                m_saveIdTime = {id, std::distance(vec.begin(), std::find(vec.begin(), vec.end(),
                                                  std::pair{m_params.mainTime, m_params.minorTime}))
                               };
                m_timeChessButtons->useButton(m_saveIdTime.second);
            }
        }
    }

    m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);

    connect(m_gameTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();
        m_params.gameType = static_cast<TypeGame>(id);
    });
    connect(m_chessTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();
        m_params.chessType = static_cast<TypeChess>(id);
    });
    connect(m_timeChessTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
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
            m_stackedTime->setCurrentWidget(m_timeChessButtons);
            m_stackedTime->show();

            m_timeChessButtons->setButtons(m_constans->getTimeChessStr()[id]);
            if (id == m_saveIdTime.first) {
                m_timeChessButtons->useButton(m_saveIdTime.second);
                const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][m_saveIdTime.second];
                m_params.mainTime = time.first;
                m_params.minorTime = time.second;
            }
        }
    });
    connect(m_timeChessButtons, &ButtonComplex::selectButtonSignals, this, [this](qint8 id) {
        m_errorLabel->clear();

        m_saveIdTime = {static_cast<qint8>(m_params.timeChessType), id};
        const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][id];
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
        m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);
    });
    connect(m_timeChessSpins, &TimeChess::changeTimeSignal, this, [this]() {
        m_errorLabel->clear();

        m_saveIdTime = {EMPTY, EMPTY};
        const auto &time = m_timeChessSpins->getTime();
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
    });
    connect(m_startGame, &QPushButton::clicked, this, [this]() {
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

StartParams &StartGameWindow::getStartParams()
{
    return m_params;
}
