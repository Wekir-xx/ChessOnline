#include "start_game_window.h"

StartGameWindow::StartGameWindow(QWidget *parent)
    : QWidget{parent}
{
    m_mainLayout = new QVBoxLayout();

    m_gameTypeButtons = new ButtonComplex();
    m_chessTypeButtons = new ButtonComplex();
    m_timeChessTypeButtons = new ButtonComplex();
    m_timeChessButtons = new ButtonComplex();
    m_timeChessSpin = new TimeChess();
    m_errorLabel = new QLabel();
    m_startGame = new QPushButton("Start Game");

    m_constans = &SomeConstans::getInstance();
    m_gameTypeButtons->setButtons(m_constans->getTypeGameStr());
    m_chessTypeButtons->setButtons(m_constans->getTypeChessStr());
    m_timeChessTypeButtons->setButtons(m_constans->getTypeTimeChessStr());
    m_timeChessSpin->setTime(0, 0, 0, 0);

    m_errorLabel->setStyleSheet("color: red;"
                                "font-size: 18px;");
    m_startGame->setFixedHeight(FIXED_SIZE_BUTTON);

    m_mainLayout->setSpacing(0);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_gameTypeButtons);
    m_mainLayout->addWidget(m_chessTypeButtons);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_timeChessTypeButtons);
    m_mainLayout->addStretch(1);
    m_mainLayout->addWidget(m_errorLabel);
    m_mainLayout->addWidget(m_startGame);

    m_mainLayout->setAlignment(m_errorLabel, Qt::AlignCenter);

    connect(m_gameTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](size_t num) {
        m_errorLabel->clear();
        m_params.gameType = static_cast<TypeGame>(num);
    });
    connect(m_chessTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](size_t num) {
        m_errorLabel->clear();
        m_params.chessType = static_cast<TypeChess>(num);
    });
    connect(m_timeChessTypeButtons, &ButtonComplex::selectButtonSignals, this, [this](size_t num) {
        m_errorLabel->clear();
        m_params.timeChessType = static_cast<TypeTimeChess>(num);

        if (m_timeChessButtons->parent())
            m_timeChessButtons->setParent(nullptr);
        if (m_timeChessSpin->parent())
            m_timeChessSpin->setParent(nullptr);

        int id = m_mainLayout->indexOf(m_timeChessTypeButtons) + 1;
        if (m_params.timeChessType == TypeTimeChess::OTHER) {
            m_mainLayout->insertWidget(id, m_timeChessSpin);
            m_mainLayout->setAlignment(m_timeChessSpin, Qt::AlignCenter);
        } else if (m_params.timeChessType == TypeTimeChess::NO_TIME) {
            m_params.mainTime = 0;
            m_params.minorTime = 0;
        } else {
            m_mainLayout->insertWidget(id, m_timeChessButtons);
            m_timeChessButtons->setButtons(m_constans->getTimeChessStr()[num]);
        }
    });
    connect(m_timeChessButtons, &ButtonComplex::selectButtonSignals, this, [this](size_t num) {
        m_errorLabel->clear();
        const auto &time = m_constans->getTimeChessValue()[static_cast<size_t>(m_params.timeChessType)][num];
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
    });
    connect(m_timeChessSpin, &TimeChess::changeTimeSignal, this, [this]() {
        m_errorLabel->clear();
        const auto &time = m_timeChessSpin->getTime();
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
