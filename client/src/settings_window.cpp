#include "settings_window.h"

SettingsWindow::SettingsWindow(SettingsParams params, QWidget *parent)
    : QWidget{parent}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(true);
    this->setStyleSheet("SettingsWindow {"
                        "   border: 2px solid #808080;"
                        "   border-radius: 10px;"
                        "}");

    QVBoxLayout *layoutV = new QVBoxLayout();
    QHBoxLayout *layoutH = new QHBoxLayout();

    QPushButton *exit = new QPushButton();
    QPushButton *turnBoardBut = new QPushButton("Turn board");
    QPushButton *turnChessBut = new QPushButton("Turn second player");
    QCheckBox *autoQueenBut = new QCheckBox("Auto queen");
    QCheckBox *premoveBut = new QCheckBox("Premove");
    QCheckBox *noticeTimeBut = new QCheckBox("Notice small time");

    exit->setIcon(QIcon(pathGeneral + "exit.png"));
    exit->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    exit->setIconSize(exit->size());
    exit->setStyleSheet("padding: 0px;");

    layoutH->addStretch();
    layoutH->addWidget(exit, 1);
    layoutV->addLayout(layoutH);
    layoutV->addWidget(turnBoardBut, 2);
    layoutV->addWidget(turnChessBut, 2);
    layoutV->addWidget(autoQueenBut, 2);
    if (params.gameType == TypeGame::ONLINE)
        layoutV->addWidget(premoveBut, 2);
    layoutV->addWidget(noticeTimeBut, 2);

    autoQueenBut->setChecked(params.checkAutoQueen);
    premoveBut->setChecked(params.checkPremove);
    noticeTimeBut->setChecked(params.checkNoticeTime);

    connect(exit, &QPushButton::clicked, this, &SettingsWindow::exitSignal);
    connect(turnBoardBut, &QPushButton::clicked, this, &SettingsWindow::turnBoard);
    connect(turnChessBut, &QPushButton::clicked, this, &SettingsWindow::turnChess);
    connect(autoQueenBut, &QCheckBox::clicked, this, &SettingsWindow::autoQueen);
    connect(premoveBut, &QCheckBox::clicked, this, &SettingsWindow::premove);
    connect(noticeTimeBut, &QCheckBox::clicked, this, &SettingsWindow::noticeTime);

    this->setLayout(layoutV);
}
