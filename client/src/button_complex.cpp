#include "button_complex.h"

ButtonComplex::ButtonComplex(QWidget *parent)
    : QWidget{parent}
{
    m_buttons.reserve(10);

    m_layout = new QHBoxLayout();
    m_layout->setSpacing(0);
    this->setLayout(m_layout);
}

void ButtonComplex::setButtons(std::vector<std::string> nameButtons)
{
    size_t i = 0;
    for (; i < nameButtons.size() && i < m_buttons.size(); ++i) {
        m_buttons[i]->setText(QString::fromStdString(nameButtons[i]));
        m_buttons[i]->show();
    }

    for (; i < nameButtons.size(); ++i) {
        m_buttons.push_back(new QPushButton(QString::fromStdString(nameButtons[i])));
        m_layout->addWidget(m_buttons[i]);

        connect(m_buttons[i], &QPushButton::clicked, this, [=]() {
            unUseButton();
            useButton(i);
            emit selectButtonSignals(i);
        });
    }

    for (; i < m_buttons.size(); ++i)
        m_buttons[i]->hide();

    m_numUseButton = i;
}

void ButtonComplex::useButton(size_t num)
{
    m_numUseButton = num;
    m_buttons[num]->setChecked(true);
    m_buttons[num]->setEnabled(false);
}

void ButtonComplex::unUseButton()
{
    if (m_numUseButton < m_buttons.size()) {
        m_buttons[m_numUseButton]->setChecked(false);
        m_buttons[m_numUseButton]->setEnabled(true);
    }
}
