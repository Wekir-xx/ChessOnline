#ifndef BUTTON_COMPLEX_H
#define BUTTON_COMPLEX_H

#include "src/style_lib.h"

#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <vector>

class ButtonComplex : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonComplex(StyleLib *styleLib, QWidget *parent = nullptr);

    void setButtons(std::vector<std::string> nameButtons);
    void useButton(qint8 id);

signals:
    void selectButton(qint8 id);

private:
    void unUseButton();
    void setStyle();

private:
    StyleLib *m_styleLib;

    QHBoxLayout *m_layout;

    std::vector<QPushButton *> m_buttons;
    size_t m_idUseBut;
};

#endif // BUTTON_COMPLEX_H
