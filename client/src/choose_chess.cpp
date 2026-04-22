#include "choose_chess.h"

#include <QIcon>

ChooseChess::ChooseChess(StyleLib *styleLib, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
{
    m_buttons.resize(12, nullptr);
    m_idUseBut = m_buttons.size();
    m_constans = &SomeConstans::getInstance();

    const auto &pieceKeysWhite = m_constans->getPieceKeysWhite();
    const auto &pieceKeysBlack = m_constans->getPieceKeysBlack();
    const auto &path = m_styleLib->getIconStyle();

    m_mainLayout = new QGridLayout();

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < pieceKeysWhite.size(); ++j) {
            size_t id = i * pieceKeysWhite.size() + j;
            m_buttons[id] = new QPushButton();
            m_buttons[id]->setSizePolicy(sizePolicy);
            m_buttons[id]->setMinimumSize(MINIMUM_PIECE_SIZE, MINIMUM_PIECE_SIZE);
            m_buttons[id]->setStyleSheet("padding: 0px; border: none;");

            m_mainLayout->addWidget(m_buttons[id], j, i);

            if (i == 0) {
                m_buttons[id]->setObjectName(pieceKeysWhite[j]);
                m_buttons[id]->setIcon(QIcon(path + pieceKeysWhite[j]));
            } else {
                m_buttons[id]->setObjectName(pieceKeysBlack[j]);
                m_buttons[id]->setIcon(QIcon(path + pieceKeysBlack[j]));
            }

            connect(m_buttons[id], &QPushButton::clicked, this, [this, id]() {
                if (m_idUseBut != m_buttons.size())
                    m_buttons[m_idUseBut]->setChecked(true);

                if (id != m_idUseBut) {
                    m_idUseBut = id;
                    m_buttons[id]->setChecked(false);
                } else {
                    m_idUseBut = m_buttons.size();
                }

                emit selectPiece(m_buttons[id]->objectName());
            });
        }
    }

    this->setLayout(m_mainLayout);
}

void ChooseChess::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->updateSize();
}

void ChooseChess::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->updateSize();
}

void ChooseChess::updateSize()
{
    for (const auto &piece : m_buttons)
        piece->setIconSize(piece->size());
}
