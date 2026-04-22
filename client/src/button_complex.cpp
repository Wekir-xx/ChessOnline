#include "button_complex.h"

ButtonComplex::ButtonComplex(StyleLib *styleLib, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
{
    m_buttons.reserve(10);

    m_layout = new QHBoxLayout();
    m_layout->setSpacing(0);
    this->setLayout(m_layout);
}

void ButtonComplex::setButtons(std::vector<std::string> nameButtons)
{
    unUseButton();
    size_t i = 0;
    for (; i < nameButtons.size() && i < m_buttons.size(); ++i) {
        m_buttons[i]->setText(QString::fromStdString(nameButtons[i]));
        m_buttons[i]->show();
    }

    for (; i < nameButtons.size(); ++i) {
        m_buttons.push_back(new QPushButton(QString::fromStdString(nameButtons[i])));
        m_layout->addWidget(m_buttons[i]);

        connect(m_buttons[i], &QPushButton::clicked, this, [ = ]() {
            unUseButton();
            useButton(i);
            emit selectButton(i);
        });
    }

    for (; i < m_buttons.size(); ++i)
        m_buttons[i]->hide();

    m_idUseBut = i;
}

void ButtonComplex::useButton(qint8 id)
{
    m_idUseBut = id;
    m_buttons[id]->setEnabled(false);
}

void ButtonComplex::unUseButton()
{
    if (m_idUseBut < m_buttons.size())
        m_buttons[m_idUseBut]->setEnabled(true);
}
