#ifndef PLAYER_INFO_LAYOUT3_H
#define PLAYER_INFO_LAYOUT3_H

#include <QVBoxLayout>
#include <QWidget>

class PlayerInfoLayout3 : public QVBoxLayout
{
public:
    using QVBoxLayout::QVBoxLayout;

    void swapOrientation()
    {
        m_orientationUp ^= true;
    }

    void setGeometry(const QRect &rect) override
    {
        QVBoxLayout::setGeometry(rect);

        if (count() < 3)
            return;

        QLayoutItem *firstItem = itemAt(0);
        QLayoutItem *secondItem = itemAt(1);
        QLayoutItem *thirdItem = itemAt(2);
        QWidget *mainWidget;
        QWidget *otherWidget1 = secondItem->widget();
        QWidget *otherWidget2;

        if (m_orientationUp) {
            mainWidget = firstItem->widget();
            otherWidget2 = thirdItem->widget();
        } else {
            mainWidget = thirdItem->widget();
            otherWidget2 = firstItem->widget();
        }

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int fixedHeight1 = otherWidget1->sizeHint().height();
        int fixedHeight2 = otherWidget2->sizeHint().height();
        int width = adjustedRect.width();
        int height = adjustedRect.height();
        int newSide = qMin(width, height - fixedHeight1 - fixedHeight2);

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        if (m_orientationUp) {
            mainWidget->setGeometry(QRect(x, y, newSide, newSide));
            otherWidget1->setGeometry(QRect(x, y + newSide, newSide, fixedHeight1));
            otherWidget2->setGeometry(QRect(x, y + newSide + fixedHeight1, width, fixedHeight2));
        } else {
            otherWidget2->setGeometry(QRect(x, height - newSide - fixedHeight1 - fixedHeight2, width, fixedHeight2));
            otherWidget1->setGeometry(QRect(x, height - newSide - fixedHeight1, newSide, fixedHeight1));
            mainWidget->setGeometry(QRect(x, height - newSide, newSide, newSide));
        }
    }

private:
    bool m_orientationUp{true};
};

#endif // PLAYER_INFO_LAYOUT3_H
