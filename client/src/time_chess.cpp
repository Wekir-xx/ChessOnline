#include "time_chess.h"

TimeChess::TimeChess(QWidget *parent)
    : QWidget{parent}
{
    m_block = false;

    m_mainLayout = new QHBoxLayout();

    m_timeH = new QSpinBox();
    m_timeM = new QSpinBox();
    m_timeS = new QSpinBox();
    m_timeMinorS = new QSpinBox();

    m_timeH->setRange(0, 72);
    m_timeM->setRange(0, 60);
    m_timeS->setRange(0, 60);
    m_timeMinorS->setRange(0, 120);

    m_timeH->setSuffix(" hour");
    m_timeM->setSuffix(" min");
    m_timeS->setSuffix(" sec");
    m_timeMinorS->setSuffix(" sec");

    m_mainLayout->addWidget(m_timeH);
    m_mainLayout->addWidget(m_timeM);
    m_mainLayout->addWidget(m_timeS);
    m_mainLayout->addWidget(m_timeMinorS);

    connect(m_timeH, &QSpinBox::valueChanged, this, [this]() {
        if (!m_block)
            emit changeTimeSignal();
    });
    connect(m_timeM, &QSpinBox::valueChanged, this, [this]() {
        if (!m_block)
            emit changeTimeSignal();
    });
    connect(m_timeS, &QSpinBox::valueChanged, this, [this]() {
        if (!m_block)
            emit changeTimeSignal();
    });
    connect(m_timeMinorS, &QSpinBox::valueChanged, this, [this]() {
        if (!m_block)
            emit changeTimeSignal();
    });

    this->setLayout(m_mainLayout);
}

void TimeChess::setTime(qint32 mainTime, qint8 minorTime)
{
    m_block = true;
    m_timeH->setValue(mainTime / 60 / 60);
    m_timeM->setValue(mainTime / 60 % 60);
    m_timeS->setValue(mainTime % 60);
    m_timeMinorS->setValue(minorTime);
    m_block = false;
}

std::pair<qint32, qint8> TimeChess::getTime()
{
    return std::pair{(m_timeH->value() * 60 + m_timeM->value()) * 60 + m_timeS->value(), m_timeMinorS->value()};
}
