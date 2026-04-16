#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include <QHBoxLayout>
#include <QWidget>

class BoardLayout : public QHBoxLayout
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
        QLayout *otherWidget = secondItem->layout();

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int fixedWidth = otherWidget->sizeHint().width();
        int width = adjustedRect.width() - fixedWidth;
        int height = adjustedRect.height();
        int newSide = qMin(width, height);

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        mainWidget->setGeometry(QRect(x, y, newSide, newSide));
        otherWidget->setGeometry(QRect(x + newSide, y, fixedWidth, newSide));
    }
};

#endif // BOARD_LAYOUT_H
