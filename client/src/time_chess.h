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

    void setTime(qint32 mainTime, qint8 minorTime);
    std::pair<qint32, qint8> getTime();

signals:
    void changeTime();

private:
    QHBoxLayout *m_mainLayout;

    QSpinBox *m_timeH;
    QSpinBox *m_timeM;
    QSpinBox *m_timeS;
    QSpinBox *m_timeMinorS;

    bool m_block;
};

#endif // TIME_CHESS_H
