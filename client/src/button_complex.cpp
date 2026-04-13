#include "button_complex.h"

ButtonComplex::ButtonComplex(QWidget *parent)
    : QWidget{parent}
{
    m_layout = new QHBoxLayout();
    m_layout->setSpacing(0);
    this->setLayout(m_layout);
}

void ButtonComplex::setButtons(std::vector<std::string> nameButtons)
{
    for (const auto &button : m_buttons)
        button->deleteLater();
    m_buttons.clear();

    m_numUseButton = nameButtons.size();
    m_buttons.reserve(m_numUseButton);
    for (size_t i = 0; i < m_numUseButton; ++i) {
        m_buttons.push_back(new QPushButton(QString::fromStdString(nameButtons[i])));
        m_layout->addWidget(m_buttons[i]);

        connect(m_buttons[i], &QPushButton::clicked, this, [=]() {
            unUseButton();
            useButton(i);
            emit selectButtonSignals(i);
        });
    }
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
