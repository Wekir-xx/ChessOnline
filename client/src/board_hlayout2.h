#ifndef BOARD_HLAYOUT2_H
#define BOARD_HLAYOUT2_H

#include <QHBoxLayout>
#include <QWidget>

class BoardHLayout2 : public QHBoxLayout
{
public:
    using QHBoxLayout::QHBoxLayout;

    void setGeometry(const QRect &rect) override
    {
        if (count() < 2)
            return;

        QLayoutItem *firstItem = itemAt(0);
        QLayoutItem *secondItem = itemAt(1);

        QWidget *mainWidget = firstItem->widget();
        QWidget *otherWidget = secondItem->widget();

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int width = adjustedRect.width();
        int height = adjustedRect.height();

        int newSide = qMin(width * 4 / 5, height);

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        mainWidget->setGeometry(QRect(x, y, newSide, newSide));
        otherWidget->setGeometry(QRect(x + newSide, y, width - newSide, newSide));
    }
};

#endif // BOARD_HLAYOUT2_H
