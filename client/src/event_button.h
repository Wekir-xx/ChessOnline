#ifndef EVENT_BUTTON_H
#define EVENT_BUTTON_H

#include <QObject>
#include <QPushButton>

class EventButton : public QPushButton
{
    Q_OBJECT
public:
    using QPushButton::QPushButton;
signals:
    void pressMouseSignal(QMouseEvent *event);

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        emit pressMouseSignal(event);
        QPushButton::mousePressEvent(event);
    }
};

#endif // EVENT_BUTTON_H
