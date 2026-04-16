#include "settings_window.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(true);
    this->setStyleSheet("SettingsWindow {"
                        "   background-color: #D1D1D1;"
                        "   border: 2px solid #808080;"
                        "   border-radius: 10px;"
                        "}");

    m_layoutV = new QVBoxLayout();
    m_layoutH = new QHBoxLayout();

    m_exit = new QPushButton();
    m_turnBoardBut = new QPushButton("Turn board");
    m_turnChessBut = new QPushButton("Turn second player");
    m_exitGame = new QPushButton("Exit");
    m_hideAllBut = new QCheckBox("Hide all");
    m_autoQueenBut = new QCheckBox("Auto queen");
    m_autoRotateBut = new QCheckBox("Auto-rotate game");
    m_premoveBut = new QCheckBox("Premove", this);
    m_noticeTimeBut = new QCheckBox("Notice small time");

    m_exit->setIcon(QIcon(SomeConstans::getInstance().getPathGeneral() + "exit.png"));
    m_exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exit->setIconSize(m_exit->size());
    m_exit->setStyleSheet("padding: 0px;");

    m_layoutH->addStretch(3);
    m_layoutH->addWidget(m_hideAllBut);
    m_layoutH->addStretch(2);
    m_layoutH->addWidget(m_exit);

    m_layoutV->addLayout(m_layoutH);
    m_layoutV->addWidget(m_turnBoardBut);
    m_layoutV->addWidget(m_turnChessBut);
    m_layoutV->addWidget(m_autoQueenBut);
    m_layoutV->addWidget(m_autoRotateBut);
    m_layoutV->addWidget(m_noticeTimeBut);
    m_layoutV->addWidget(m_premoveBut);
    m_layoutV->addWidget(m_exitGame);

    connect(m_hideAllBut, &QCheckBox::clicked, this, &SettingsWindow::hideAll);
    connect(m_exit, &QPushButton::clicked, this, &SettingsWindow::exit);
    connect(m_turnBoardBut, &QPushButton::clicked, this, &SettingsWindow::turnBoard);
    connect(m_turnChessBut, &QPushButton::clicked, this, &SettingsWindow::turnSecondPlayer);
    connect(m_exitGame, &QPushButton::clicked, this, &SettingsWindow::exitGame);
    connect(m_autoQueenBut, &QCheckBox::clicked, this, &SettingsWindow::autoQueen);
    connect(m_autoRotateBut, &QCheckBox::clicked, this, &SettingsWindow::autoRotate);
    connect(m_premoveBut, &QCheckBox::clicked, this, &SettingsWindow::premove);
    connect(m_noticeTimeBut, &QCheckBox::clicked, this, &SettingsWindow::noticeTime);

    this->setLayout(m_layoutV);
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
    m_exitGame->setEnabled(flag);
}
