#include "settings_window.h"

SettingsWindow::SettingsWindow(StyleLib *styleLib, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(true);

    m_layoutV = new QVBoxLayout();
    m_layoutH = new QHBoxLayout();

    m_exitBut = new QPushButton();
    m_turnBoardBut = new QPushButton("Turn board");
    m_turnChessBut = new QPushButton("Turn second player");
    m_exitGameBut = new QPushButton("Exit");
    m_hideAllBut = new QCheckBox("Hide all");
    m_autoQueenBut = new QCheckBox("Auto queen");
    m_autoRotateBut = new QCheckBox("Auto-rotate game");
    m_premoveBut = new QCheckBox("Premove", this);
    m_noticeTimeBut = new QCheckBox("Notice small time");

    m_exitBut->setIcon(QIcon(QString(GENERAL_PATH) + "exit.png"));
    m_exitBut->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exitBut->setIconSize(m_exitBut->size());
    m_exitBut->setStyleSheet("padding: 0px;");

    m_layoutH->addStretch(3);
    m_layoutH->addWidget(m_hideAllBut);
    m_layoutH->addStretch(2);
    m_layoutH->addWidget(m_exitBut);

    m_layoutV->addLayout(m_layoutH);
    m_layoutV->addWidget(m_turnBoardBut);
    m_layoutV->addWidget(m_turnChessBut);
    m_layoutV->addWidget(m_autoQueenBut);
    m_layoutV->addWidget(m_autoRotateBut);
    m_layoutV->addWidget(m_noticeTimeBut);
    m_layoutV->addWidget(m_premoveBut);
    m_layoutV->addWidget(m_exitGameBut);

    this->setLayout(m_layoutV);
    this->setStyle();

    connect(m_hideAllBut, &QCheckBox::clicked, this, &SettingsWindow::hideAll);
    connect(m_exitBut, &QPushButton::clicked, this, &SettingsWindow::exit);
    connect(m_turnBoardBut, &QPushButton::clicked, this, &SettingsWindow::turnBoard);
    connect(m_turnChessBut, &QPushButton::clicked, this, &SettingsWindow::turnSecondPlayer);
    connect(m_exitGameBut, &QPushButton::clicked, this, &SettingsWindow::exitGame);
    connect(m_autoQueenBut, &QCheckBox::clicked, this, &SettingsWindow::autoQueen);
    connect(m_autoRotateBut, &QCheckBox::clicked, this, &SettingsWindow::autoRotate);
    connect(m_premoveBut, &QCheckBox::clicked, this, &SettingsWindow::premove);
    connect(m_noticeTimeBut, &QCheckBox::clicked, this, &SettingsWindow::noticeTime);

    connect(m_styleLib, &StyleLib::changeWindowStyle, this, &SettingsWindow::setStyle);
}

void SettingsWindow::setParams(SettingsParams params, TypeGame gameType, TypeTimeChess timeChessType)
{
    if (timeChessType == TypeTimeChess::NO_TIME)
        m_noticeTimeBut->hide();
    else
        m_noticeTimeBut->show();

    if (gameType == TypeGame::ONLINE)
        m_premoveBut->show();
    else
        m_premoveBut->hide();

    m_hideAllBut->setChecked(params.hideAll);
    m_autoQueenBut->setChecked(params.checkAutoQueen);
    m_autoRotateBut->setChecked(params.checkAutoRotate);
    m_noticeTimeBut->setChecked(params.checkNoticeTime);
    m_premoveBut->setChecked(params.checkPremove);
}

void SettingsWindow::setExitButton(bool flag)
{
    m_exitGameBut->setEnabled(flag);
}

void SettingsWindow::setStyle()
{
    this->setStyleSheet(m_styleLib->getSecondWindowStyle());

    const auto styleBut = m_styleLib->getButtonStyle();
    m_exitBut->setStyleSheet(styleBut);
    m_turnBoardBut->setStyleSheet(styleBut);
    m_turnChessBut->setStyleSheet(styleBut);
    m_exitGameBut->setStyleSheet(styleBut);

    const auto styleBox = m_styleLib->getCheckBoxStyle();
    m_hideAllBut->setStyleSheet(styleBox);
    m_autoQueenBut->setStyleSheet(styleBox);
    m_autoRotateBut->setStyleSheet(styleBox);
    m_premoveBut->setStyleSheet(styleBox);
    m_noticeTimeBut->setStyleSheet(styleBox);
}
