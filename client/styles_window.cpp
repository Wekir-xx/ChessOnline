#include "styles_window.h"

#include "src/defines.h"

StylesWindow::StylesWindow(StyleLib *styleLib, QWidget *parent)
    : QWidget{parent}
    , m_styleLib{styleLib}
{
    m_chessFields.resize(SIDE_SIZE, std::vector<QString>(SIDE_SIZE));
    SomeConstans::fillStandartChessField(m_chessFields);

    m_chessBoard = new ChessBoardWidget(m_styleLib);

    m_mainLayout = new QVBoxLayout();
    m_downLayout = new QHBoxLayout();

    m_exitBut = new QPushButton();
    m_iconStyle = new QComboBox();
    m_boardStyle = new QComboBox();
    m_windowStyle = new QComboBox();

    const auto &iconStyles = m_styleLib->getIconStyles();
    for (size_t i = 0; i < iconStyles.size(); ++i)
        m_iconStyle->addItem(iconStyles[i]);
    m_iconStyle->setCurrentIndex(m_styleLib->getIdIconStyle());

    const auto &boardStyles = m_styleLib->getBoardStyles();
    for (size_t i = 0; i < boardStyles.size(); ++i)
        m_boardStyle->addItem(boardStyles[i]);
    m_boardStyle->setCurrentIndex(m_styleLib->getIdBoardStyle());

    const auto &windowStyles = m_styleLib->getWindowStyles();
    for (size_t i = 0; i < windowStyles.size(); ++i)
        m_windowStyle->addItem(windowStyles[i]);
    m_windowStyle->setCurrentIndex(m_styleLib->getIdWindowStyle());

    m_exitBut->setIcon(QIcon(QString(GENERAL_PATH) + "exit.png"));
    m_exitBut->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exitBut->setIconSize(m_exitBut->size());

    m_downLayout->addWidget(m_exitBut);
    m_downLayout->addStretch();
    m_downLayout->addWidget(m_iconStyle);
    m_downLayout->addWidget(m_boardStyle);
    m_downLayout->addWidget(m_windowStyle);

    m_mainLayout->addWidget(m_chessBoard);
    m_mainLayout->addLayout(m_downLayout);

    this->setLayout(m_mainLayout);
    this->setStyle();

    connect(m_exitBut, &QPushButton::clicked, this, &StylesWindow::exit);
    connect(m_iconStyle, &QComboBox::currentIndexChanged, this, [this](int index) {
        m_styleLib->setIdIconStyle(index);
    });
    connect(m_boardStyle, &QComboBox::currentIndexChanged, this, [this](int index) {
        m_styleLib->setIdBoardStyle(index);
    });
    connect(m_windowStyle, &QComboBox::currentIndexChanged, this, [this](int index) {
        m_styleLib->setIdWindowStyle(index);
    });

    connect(m_styleLib, &StyleLib::changeIconStyle, this, &StylesWindow::updateIcon);
    connect(m_styleLib, &StyleLib::changeBoardStyle, this, &StylesWindow::updateBoard);
    connect(m_styleLib, &StyleLib::changeWindowStyle, this, &StylesWindow::setStyle);
}

void StylesWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->updateIcon();
    this->updateBoardSize();
}

void StylesWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->updateBoardSize();
}

void StylesWindow::updateIcon()
{
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->setIcon(i, j, m_chessFields[i][j]);
}

void StylesWindow::updateBoard()
{
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->baseField(i, j);
}

void StylesWindow::updateBoardSize()
{
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->setIconSize(i, j);
}

void StylesWindow::setStyle()
{
    this->setStyleSheet(m_styleLib->getColorTextStyle());

    m_exitBut->setStyleSheet(m_styleLib->getButtonStyle());

    const auto style = m_styleLib->getComboBoxStyle();
    m_iconStyle->setStyleSheet(style);
    m_boardStyle->setStyleSheet(style);
    m_windowStyle->setStyleSheet(style);
}
