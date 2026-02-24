#ifndef PLAYER_INFO_LAYOUT_H
#define PLAYER_INFO_LAYOUT_H

#include <QVBoxLayout>
#include <iostream>

class PlayerInfoLayout : public QVBoxLayout
{
public:
    using QVBoxLayout::QVBoxLayout;

    void setGeometry(const QRect& rect) override
    {
        QVBoxLayout::setGeometry(rect);

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

        int fixedHeight = otherWidget->size().height();
        int width = adjustedRect.right() - adjustedRect.left();

        int x = mainWidget->x();
        int y = mainWidget->y();

        mainWidget->setGeometry(QRect(x, y, width, width));
        otherWidget->setGeometry(QRect(x, y + width, width, fixedHeight));
    }
};

#endif // PLAYER_INFO_LAYOUT_H
