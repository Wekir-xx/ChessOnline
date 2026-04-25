#include "start_game_window.h"

#include <algorithm>

StartGameWindow::StartGameWindow(StyleLib *styleLib, StartParams &startParams, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
    , m_params{startParams}
{
    m_settings = new SettingsWindow(m_styleLib, this);
    m_saveIdTime = {static_cast<qint8>(TypeTimeChess::UNDEFINED), static_cast<qint8>(TypeTimeChess::UNDEFINED)};

    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_buttonLayout = new QHBoxLayout();
    m_stackedTime = new QStackedWidget();

    m_gameTypeButs = new ButtonComplex();
    m_chessTypeButs = new ButtonComplex();
    m_timeChessTypeButs = new ButtonComplex();
    m_timeChessButs = new ButtonComplex();
    m_settingBut = new QPushButton();
    m_boardSetupBut = new QPushButton("Board setup");
    m_startGameBut = new QPushButton("Start Game");
    m_timeChessSpins = new TimeChess();
    m_errorLabel = new QLabel();

    m_constans = &SomeConstans::getInstance();
    m_gameTypeButs->setButtons(m_constans->getTypeGameStr());
    m_chessTypeButs->setButtons(m_constans->getTypeChessStr());
    m_timeChessTypeButs->setButtons(m_constans->getTypeTimeChessStr());

    m_errorLabel->setStyleSheet("color: red; font-size: 18px;");
    m_settingBut->setIcon(QIcon(QString(GENERAL_PATH) + "settings.png"));
    m_settingBut->setIconSize(QSize(FIXED_SIZE_BUTTON_ICON, FIXED_SIZE_BUTTON_ICON));
    m_boardSetupBut->adjustSize();

    m_boardSetupBut->setFixedSize(m_boardSetupBut->size());
    m_settingBut->setFixedSize(FIXED_SIZE_BUTTON, FIXED_SIZE_BUTTON);
    m_startGameBut->setFixedHeight(FIXED_SIZE_BUTTON);

    m_stackedTime->addWidget(m_timeChessButs);
    m_stackedTime->addWidget(m_timeChessSpins);

    m_topLayout->addWidget(m_settingBut);
    m_topLayout->addStretch();

    m_buttonLayout->addStretch(7);
    m_buttonLayout->addWidget(m_boardSetupBut);
    m_buttonLayout->addStretch(1);

    m_mainLayout->setSpacing(0);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_gameTypeButs);
    m_mainLayout->addWidget(m_chessTypeButs);
    m_mainLayout->addLayout(m_buttonLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_timeChessTypeButs);
    m_mainLayout->addWidget(m_stackedTime);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_errorLabel);
    m_mainLayout->addWidget(m_startGameBut);

    m_mainLayout->setAlignment(m_stackedTime, Qt::AlignCenter);
    m_mainLayout->setAlignment(m_errorLabel, Qt::AlignCenter);

    if (m_params.gameType != TypeGame::UNDEFINED)
        m_gameTypeButs->useButton(static_cast<qint8>(m_params.gameType));

    if (m_params.chessType != TypeChess::UNDEFINED)
        m_chessTypeButs->useButton(static_cast<qint8>(m_params.chessType));

    if (m_params.timeChessType != TypeTimeChess::UNDEFINED) {
        qint8 id = static_cast<qint8>(m_params.timeChessType);
        m_timeChessTypeButs->useButton(id);

        if (m_params.timeChessType != TypeTimeChess::NO_TIME) {
            if (m_params.timeChessType == TypeTimeChess::OTHER) {
                m_stackedTime->setCurrentWidget(m_timeChessSpins);
            } else {
                m_stackedTime->setCurrentWidget(m_timeChessButs);
                m_timeChessButs->setButtons(m_constans->getTimeChessStr()[id]);
                m_saveIdTime.first = id;

                if (m_params.mainTime != 0) {
                    const auto &vec = m_constans->getTimeChessValue()[id];
                    size_t idTime = std::distance(vec.begin(),
                                                  std::find(vec.begin(), vec.end(), std::pair{m_params.mainTime, m_params.minorTime}));

                    if (idTime != vec.size()) {
                        m_saveIdTime.second = idTime;
                        m_timeChessButs->useButton(m_saveIdTime.second);
                    }
                }
            }
        }
    } else {
        m_params.mainTime = 0;
        m_params.minorTime = 0;
    }
    m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);

    m_settings->hide();
    this->lookBoardSetupBut();

    this->setLayout(m_mainLayout);
    this->setStyle();

    connect(m_settingBut, &QPushButton::clicked, this, [this]() {
        if (!m_settings->isVisible()) {
            m_settings->raise();
            m_settings->show();
            this->showSettingWindow();
        } else {
            m_settings->hide();
        }
    });
    connect(m_gameTypeButs, &ButtonComplex::selectButton, this, [this](qint8 id) {
        m_errorLabel->clear();

        m_params.gameType = static_cast<TypeGame>(id);
    });
    connect(m_chessTypeButs, &ButtonComplex::selectButton, this, [this](qint8 id) {
        m_errorLabel->clear();

        m_params.chessType = static_cast<TypeChess>(id);
        this->lookBoardSetupBut();
    });
    connect(m_timeChessTypeButs, &ButtonComplex::selectButton, this, [this](qint8 id) {
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
            if (id == m_saveIdTime.first && m_saveIdTime.second != static_cast<qint8>(TypeTimeChess::UNDEFINED)) {
                m_timeChessButs->useButton(m_saveIdTime.second);
                const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][m_saveIdTime.second];
                m_params.mainTime = time.first;
                m_params.minorTime = time.second;
            } else {
                m_params.mainTime = 0;
                m_params.minorTime = 0;
            }
        }
    });
    connect(m_boardSetupBut, &QPushButton::clicked, this, &StartGameWindow::boardSetup);
    connect(m_timeChessButs, &ButtonComplex::selectButton, this, [this](qint8 id) {
        m_errorLabel->clear();

        m_saveIdTime = {static_cast<qint8>(m_params.timeChessType), id};
        const auto &time = m_constans->getTimeChessValue()[m_saveIdTime.first][m_saveIdTime.second];
        m_params.mainTime = time.first;
        m_params.minorTime = time.second;
        m_timeChessSpins->setTime(m_params.mainTime, m_params.minorTime);
    });
    connect(m_timeChessSpins, &TimeChess::changeTime, this, [this]() {
        m_errorLabel->clear();

        m_saveIdTime = {static_cast<qint8>(TypeTimeChess::UNDEFINED), static_cast<qint8>(TypeTimeChess::UNDEFINED)};
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

    connect(m_settings, &SettingsWindow::exit, this, [this]() {
        m_settings->hide();
    });
    connect(m_settings, &SettingsWindow::settingStyles, this, &StartGameWindow::settingStyles);

    connect(m_styleLib, &StyleLib::changeWindowStyle, this, &StartGameWindow::setStyle);
}

StartParams &StartGameWindow::getStartParams()
{
    return m_params;
}

void StartGameWindow::showSettingWindow()
{
    const qint16 width = this->width() - MINIMUM_PIECE_SIZE;
    const qint16 height = this->height();
    m_settings->setGeometry(width / 5 + MINIMUM_PIECE_SIZE, height / 5, width * 3 / 5, height * 3 / 5);
}

void StartGameWindow::setStyle()
{
    this->setStyleSheet(m_styleLib->getColorTextStyle());

    const auto style = m_styleLib->getButtonStyle();
    m_gameTypeButs->setStyleSheet(style);
    m_chessTypeButs->setStyleSheet(style);
    m_timeChessTypeButs->setStyleSheet(style);
    m_timeChessButs->setStyleSheet(style);
    m_settingBut->setStyleSheet(style);
    m_boardSetupBut->setStyleSheet(style);
    m_startGameBut->setStyleSheet(style);

    m_timeChessSpins->setStyleSheet(m_styleLib->getSpinBoxStyle());
}

void StartGameWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (m_settings->isVisible())
        this->showSettingWindow();
}

void StartGameWindow::lookBoardSetupBut()
{
    if (m_params.chessType == TypeChess::USER)
        m_boardSetupBut->show();
    else
        m_boardSetupBut->hide();
}
