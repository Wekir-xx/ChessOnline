#include "time_widget.h"

#include "defines.h"

TimeWidget::TimeWidget(QWidget *parent)
    : QWidget{parent}
{
    m_work = false;

    m_mainLayout = new QHBoxLayout();

    m_globalTimeLabel = new QLabel();
    m_inWorkTimeLabel = new QLabel("Work time:");
    m_inWorkTimerLabel = new QLabel(QString::asprintf("%2d:%02d:%02d", 0, 0, 0));

    m_timer = new QTimer(this);
    m_timer->start(TICK);

    this->setTime();

    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_globalTimeLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_inWorkTimeLabel);
    m_mainLayout->addWidget(m_inWorkTimerLabel);
    m_mainLayout->addStretch();

    this->setLayout(m_mainLayout);

    connect(m_timer, &QTimer::timeout, this, &TimeWidget::setTime);
}

void TimeWidget::startTime()
{
    m_work = true;
    m_startWork = QDateTime::currentDateTime();
}

void TimeWidget::stopTime()
{
    m_work = false;
}

QString TimeWidget::getTime(QDateTime time)
{
    return time.toString("dd.MM.yyyy HH:mm:ss");
}

void TimeWidget::setTime()
{
    QDateTime now = QDateTime::currentDateTime();
    m_globalTimeLabel->setText(this->getTime(now));

    if (m_work) {
        size_t resultTime = m_startWork.secsTo(now);
        const qint8 s = resultTime % 60;
        const qint8 m = resultTime / 60 % 60;
        const qint16 h = resultTime / 60 / 60;

        m_inWorkTimerLabel->setText(QString::asprintf("%2d:%02d:%02d", h, m, s));
    }
}
