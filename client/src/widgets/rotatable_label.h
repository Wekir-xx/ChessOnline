#ifndef ROTATABLE_LABEL_H
#define ROTATABLE_LABEL_H

#include <QLabel>
#include <QPainter>

class RotatableLabel : public QLabel
{
public:
    explicit RotatableLabel(QWidget *parent = nullptr) : QLabel(parent) {}

    void swapRotatable()
    {
        m_rotatable ^= true;
    }

protected:
    void paintEvent([[maybe_unused]] QPaintEvent *event) override
    {
        QPainter painter(this);
        painter.translate(width() / 2, height() / 2);
        painter.rotate(m_rotatable ? 180 : 0);
        painter.translate(-width() / 2, -height() / 2);
        painter.drawText(rect(), alignment(), text());
    }

private:
    bool m_rotatable{false};
};

#endif // ROTATABLE_LABEL_H
