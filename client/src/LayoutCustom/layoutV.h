#ifndef LAYOUTV_H
#define LAYOUTV_H

#include <QVBoxLayout>

class SquareVBoxLayout : public QVBoxLayout
{
public:
    using QVBoxLayout::QVBoxLayout;

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

        int fixedHeight = otherWidget->height();
        int width = adjustedRect.right() - adjustedRect.left();
        int heigth = adjustedRect.bottom() - adjustedRect.top() - fixedHeight;
        int newSide = qMin(width, heigth);

        int x = mainWidget->x();
        int y = mainWidget->y();

        mainWidget->setGeometry(QRect(x, y, newSide, newSide));
        otherWidget->setGeometry(QRect(x, y + newSide, width, heigth - newSide + fixedHeight));
    }
};

#endif // LAYOUTV_H
