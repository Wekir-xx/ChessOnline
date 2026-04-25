#include "settings_window.h"

#include "defines.h"

SettingsWindow::SettingsWindow(StyleLib *styleLib, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setAutoFillBackground(true);

    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();

    m_exitBut = new QPushButton();
    m_settingStylesBut = new QPushButton("Setting styles");

    m_exitBut->setIcon(QIcon(QString(GENERAL_PATH) + "exit.png"));
    m_exitBut->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exitBut->setIconSize(m_exitBut->size());

    m_topLayout->addStretch();
    m_topLayout->addWidget(m_exitBut);

    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_settingStylesBut);
    m_mainLayout->addStretch();

    this->setLayout(m_mainLayout);
    this->setStyle();

    connect(m_exitBut, &QPushButton::clicked, this, &SettingsWindow::exit);
    connect(m_settingStylesBut, &QPushButton::clicked, this, &SettingsWindow::settingStyles);

    connect(m_styleLib, &StyleLib::changeWindowStyle, this, &SettingsWindow::setStyle);
}

void SettingsWindow::setStyle()
{
    this->setStyleSheet(m_styleLib->getSecondWindowStyle());

    const auto styleBut = m_styleLib->getButtonStyle();
    m_exitBut->setStyleSheet(styleBut);
    m_settingStylesBut->setStyleSheet(styleBut);
}
