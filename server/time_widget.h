#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QTimer>

class TimeWidget : public QWidget
{
public:
    explicit TimeWidget(QWidget *parent = nullptr);

    void startTime();
    void stopTime();

    QString getTime(QDateTime time);

private:
    void setTime();

private:
    QHBoxLayout *m_mainLayout;

    QLabel *m_globalTimeLabel;
    QLabel *m_inWorkTimeLabel;
    QLabel *m_inWorkTimerLabel;

    QDateTime m_startWork;
    QTimer *m_timer;

    bool m_work;
};

#endif // TIMEWIDGET_H
