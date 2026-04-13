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
    m_autoQueenBut = new QCheckBox("Auto queen");
    m_autoRotateBut = new QCheckBox("Auto-rotate game");
    m_premoveBut = new QCheckBox("Premove", this);
    m_noticeTimeBut = new QCheckBox("Notice small time");

    m_exit->setIcon(QIcon(pathGeneral + "exit.png"));
    m_exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exit->setIconSize(m_exit->size());
    m_exit->setStyleSheet("padding: 0px;");

    m_layoutH->addStretch();
    m_layoutH->addWidget(m_exit, 1);

    m_layoutV->addLayout(m_layoutH);
    m_layoutV->addWidget(m_turnBoardBut, 2);
    m_layoutV->addWidget(m_turnChessBut, 2);
    m_layoutV->addWidget(m_autoQueenBut, 2);
    m_layoutV->addWidget(m_autoRotateBut, 2);
    m_layoutV->addWidget(m_noticeTimeBut, 2);

    connect(m_exit, &QPushButton::clicked, this, &SettingsWindow::exitSignal);
    connect(m_turnBoardBut, &QPushButton::clicked, this, &SettingsWindow::turnBoard);
    connect(m_turnChessBut, &QPushButton::clicked, this, &SettingsWindow::turnSecondPlayer);
    connect(m_autoQueenBut, &QCheckBox::clicked, this, &SettingsWindow::autoQueen);
    connect(m_autoRotateBut, &QCheckBox::clicked, this, &SettingsWindow::autoRotate);
    connect(m_premoveBut, &QCheckBox::clicked, this, &SettingsWindow::premove);
    connect(m_noticeTimeBut, &QCheckBox::clicked, this, &SettingsWindow::noticeTime);

    this->setLayout(m_layoutV);
}

void SettingsWindow::setParams(SettingsParams params, TypeGame gameType)
{
    if (gameType == TypeGame::ONLINE) {
        m_layoutV->addWidget(m_premoveBut, 2);
        m_premoveBut->setChecked(params.checkPremove);
    }

    m_autoQueenBut->setChecked(params.checkAutoQueen);
    m_autoRotateBut->setChecked(params.checkAutoRotate);
    m_noticeTimeBut->setChecked(params.checkNoticeTime);
}
