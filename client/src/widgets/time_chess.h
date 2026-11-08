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

    void setTime(int mainTime, int minorTime);
    std::pair<int, int> getTime();

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
