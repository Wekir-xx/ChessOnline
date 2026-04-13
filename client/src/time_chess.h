#ifndef TIME_CHESS_H
#define TIME_CHESS_H

#include <QHBoxLayout>
#include <QObject>
#include <QSpinBox>
#include <QWidget>

class TimeChess : public QWidget
{
    Q_OBJECT

public:
    explicit TimeChess(QWidget *parent = nullptr);

    void setTime(qint8 hour, qint8 min, qint8 sec, qint8 minorSec);
    std::pair<qint32, qint8> getTime();

signals:
    void changeTimeSignal();

private:
    QHBoxLayout *m_mainLayout;

    QSpinBox *m_timeH;
    QSpinBox *m_timeM;
    QSpinBox *m_timeS;
    QSpinBox *m_timeMinorS;
};

#endif // TIME_CHESS_H
