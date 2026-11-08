#ifndef BOARD_VLAYOUT2_H
#define BOARD_VLAYOUT2_H

#include <QVBoxLayout>
#include <QWidget>

class BoardVLayout2 : public QVBoxLayout
{
public:
    using QVBoxLayout::QVBoxLayout;

    void setGeometry(const QRect &rect) override
    {
        if (count() < 2)
            return;

        QLayoutItem *firstItem = itemAt(0);
        QLayoutItem *secondItem = itemAt(1);

        QLayout *mainWidget = secondItem->layout();
        QLayout *otherWidget = firstItem->layout();

        QRect adjustedRect = rect.adjusted(contentsMargins().left(),
                                           contentsMargins().top(),
                                           -contentsMargins().right(),
                                           -contentsMargins().bottom());

        int fixedHeight = otherWidget->sizeHint().height();
        int width = adjustedRect.width();
        int height = adjustedRect.height();

        int x = adjustedRect.x();
        int y = adjustedRect.y();

        otherWidget->setGeometry(QRect(x, y, width, fixedHeight));
        mainWidget->setGeometry(QRect(x, y + fixedHeight, width, height - fixedHeight));
    }
};

#endif // BOARD_VLAYOUT2_H
