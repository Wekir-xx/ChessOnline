#ifndef PLAYER_INFO_LAYOUT2_H
#define PLAYER_INFO_LAYOUT2_H

#include <QVBoxLayout>
#include <QWidget>

class PlayerInfoLayout2 : public QVBoxLayout
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
        int width = adjustedRect.width();

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        mainWidget->setGeometry(QRect(x, y, width, width));
        otherWidget->setGeometry(QRect(x, y + width, width, fixedHeight));
    }
};

#endif // PLAYER_INFO_LAYOUT2_H
