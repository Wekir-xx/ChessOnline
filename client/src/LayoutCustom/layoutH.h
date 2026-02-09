#ifndef LAYOUTH_H
#define LAYOUTH_H

#include <QHBoxLayout>

class SquareHBoxLayout : public QHBoxLayout
{
public:
    using QHBoxLayout::QHBoxLayout;

    void setGeometry(const QRect& rect) override
    {
        if (count() == 0)
            return;

        QLayoutItem* firstItem = itemAt(0);
        QLayoutItem* secondItem = itemAt(1);
        QWidget* mainWidget = firstItem->widget();
        QWidget* otherWidget = secondItem->widget();

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int fixedWidth = otherWidget->width();
        int width = adjustedRect.right() - adjustedRect.left() - fixedWidth;
        int heigth = adjustedRect.bottom() - adjustedRect.top();
        int newSide = qMin(width, heigth);

        int x = mainWidget->x();
        int y = mainWidget->y();

        mainWidget->setGeometry(QRect(x, y, newSide, newSide));
        otherWidget->setGeometry(QRect(x + newSide, y, width - newSide + fixedWidth, heigth));
    }
};

#endif // LAYOUTH_H
