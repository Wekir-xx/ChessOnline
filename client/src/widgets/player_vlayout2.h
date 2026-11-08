#ifndef PLAYER_VLAYOUT2_H
#define PLAYER_VLAYOUT2_H

#include <QVBoxLayout>
#include <QWidget>

class PlayerVLayout2 : public QVBoxLayout
{
public:
    using QVBoxLayout::QVBoxLayout;

    void setGeometry(const QRect& rect) override
    {
        QVBoxLayout::setGeometry(rect);

        if (count() < 2)
            return;

        QLayoutItem* firstItem = itemAt(0);
        QLayoutItem* secondItem = itemAt(1);

        QWidget* mainWidget = firstItem->widget();
        QWidget* otherWidget = secondItem->widget();

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int fixedHeight = otherWidget->sizeHint().height();
        int height = adjustedRect.height() - fixedHeight;
        int width = adjustedRect.width();
        int newSide = qMin(width, height);

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        mainWidget->setGeometry(QRect(x, y, newSide, newSide));
        otherWidget->setGeometry(QRect(x, y + newSide, width, fixedHeight));
    }
};

#endif // PLAYER_VLAYOUT2_H
