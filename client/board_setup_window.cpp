#include "board_setup_window.h"

#include <QPainter>
#include <QPixmap>

BoardSetupWindow::BoardSetupWindow(ChessBoardParams &boardParams, QWidget *parent)
    : QWidget{parent}
    , m_boardParams{boardParams}
{
    QString path = SomeConstans::getInstance().getPathGeneral();
    QString fontSize = SomeConstans::getInstance().getFontSizeStyle();

    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_colorLayout = new QVBoxLayout();
    m_helperLayout = new QVBoxLayout();
    m_960Layout = new QVBoxLayout();
    m_saveLayout = new QVBoxLayout();
    m_castlingLayout = new QGridLayout();

    m_exitBut = new QPushButton();
    m_saveBut = new QPushButton("Save");
    m_resetBut = new QPushButton("Reset");
    m_reverseBut = new QPushButton("Reverse");
    m_update960But = new QPushButton("New pos");
    m_whiteCastlingBut1 = new QCheckBox("0-0-0");
    m_whiteCastlingBut2 = new QCheckBox("0-0");
    m_blackCastlingBut1 = new QCheckBox("0-0-0");
    m_blackCastlingBut2 = new QCheckBox("0-0");
    m_960But = new QCheckBox("960 chess");
    m_whiteBut = new QRadioButton();
    m_blackBut = new QRadioButton();
    m_whiteCastlingLabel = new QLabel("White castling:");
    m_blackCastlingLabel = new QLabel("Black castling:");
    m_errorLabel = new QLabel();

    m_saveBut->setStyleSheet(fontSize);
    m_resetBut->setStyleSheet(fontSize);
    m_reverseBut->setStyleSheet(fontSize);
    m_update960But->setStyleSheet(fontSize);
    m_960But->setStyleSheet(fontSize);
    m_whiteCastlingLabel->setStyleSheet(fontSize);
    m_blackCastlingLabel->setStyleSheet(fontSize);
    m_errorLabel->setStyleSheet("color: red; font-size: 18px;");

    m_whiteBut->setFixedWidth(50);
    m_blackBut->setFixedWidth(50);

    if (m_boardParams.whiteMove)
        m_whiteBut->setChecked(true);
    else
        m_blackBut->setChecked(true);

    if (!m_boardParams.chess960) {
        m_update960But->setChecked(true);
        m_update960But->setEnabled(false);
    }

    m_960But->setChecked(m_boardParams.chess960);
    m_whiteCastlingBut1->setChecked(m_boardParams.castling.first.first);
    m_whiteCastlingBut2->setChecked(m_boardParams.castling.first.second);
    m_blackCastlingBut1->setChecked(m_boardParams.castling.second.first);
    m_blackCastlingBut2->setChecked(m_boardParams.castling.second.second);

    QPixmap pixmapWhite(16, 16);
    QPixmap pixmapBlack(16, 16);

    pixmapWhite.fill(Qt::transparent);
    pixmapBlack.fill(Qt::transparent);

    QPainter painterWhite(&pixmapWhite);
    QPainter painterBlack(&pixmapBlack);

    painterWhite.setPen(QPen(Qt::black, 1));
    painterBlack.setPen(QPen(Qt::white, 1));
    painterWhite.setBrush(Qt::white);
    painterBlack.setBrush(Qt::black);
    painterWhite.drawRect(1, 1, 14, 14);
    painterBlack.drawRect(1, 1, 14, 14);
    painterWhite.end();
    painterBlack.end();

    m_exitBut->setFixedSize(FIXED_SIZE_EXIT_BUTTON, FIXED_SIZE_EXIT_BUTTON);
    m_exitBut->setStyleSheet("padding: 0px;");

    m_whiteBut->setIcon(QIcon(pixmapWhite));
    m_blackBut->setIcon(QIcon(pixmapBlack));
    m_exitBut->setIcon(QIcon(path + "exit.png"));

    m_whiteBut->setIconSize(pixmapWhite.size());
    m_blackBut->setIconSize(pixmapBlack.size());
    m_exitBut->setIconSize(m_exitBut->size());

    m_colorLayout->addWidget(m_whiteBut);
    m_colorLayout->addWidget(m_blackBut);

    m_helperLayout->addWidget(m_reverseBut);
    m_helperLayout->addWidget(m_resetBut);

    m_960Layout->addWidget(m_960But);
    m_960Layout->addWidget(m_update960But);

    m_castlingLayout->setSpacing(0);
    m_castlingLayout->addWidget(m_whiteCastlingLabel, 0, 0);
    m_castlingLayout->addWidget(m_whiteCastlingBut1, 0, 1);
    m_castlingLayout->addWidget(m_whiteCastlingBut2, 0, 2);
    m_castlingLayout->addWidget(m_blackCastlingLabel, 1, 0);
    m_castlingLayout->addWidget(m_blackCastlingBut1, 1, 1);
    m_castlingLayout->addWidget(m_blackCastlingBut2, 1, 2);

    m_saveLayout->addWidget(m_saveBut);
    m_saveLayout->addWidget(m_errorLabel);

    m_topLayout->addWidget(m_exitBut);
    m_topLayout->addStretch();
    m_topLayout->addLayout(m_colorLayout);
    m_topLayout->addStretch();
    m_topLayout->addLayout(m_helperLayout);
    m_topLayout->addStretch();
    m_topLayout->addLayout(m_960Layout);
    m_topLayout->addStretch();
    m_topLayout->addLayout(m_castlingLayout);
    m_topLayout->addStretch();
    m_topLayout->addLayout(m_saveLayout);

    m_mainLayout->addStretch(10);
    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addStretch(10);

    this->setLayout(m_mainLayout);

    connect(m_exitBut, &QPushButton::clicked, this, [this]() {
        emit exit();
    });
    connect(m_saveBut, &QPushButton::clicked, this, [this]() {
        if (!m_whiteBut->isChecked() && !m_blackBut->isChecked()) {
            m_errorLabel->setText("First move color not selected");
            return;
        }

        emit saveParams();
    });
    connect(m_update960But, &QPushButton::clicked, this, [this]() { m_errorLabel->clear(); });
    connect(m_resetBut, &QPushButton::clicked, this, [this]() { m_errorLabel->clear(); });
    connect(m_reverseBut, &QPushButton::clicked, this, [this]() {});
    connect(m_whiteCastlingBut1, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.castling.first.first ^= true;
    });
    connect(m_whiteCastlingBut2, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.castling.first.second ^= true;
    });
    connect(m_blackCastlingBut1, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.castling.second.first ^= true;
    });
    connect(m_blackCastlingBut2, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.castling.second.second ^= true;
    });
    connect(m_960But, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.chess960 ^= true;
        if (!m_boardParams.chess960) {
            m_update960But->setChecked(true);
            m_update960But->setEnabled(false);
        } else {
            m_update960But->setChecked(false);
            m_update960But->setEnabled(true);
        }
    });
    connect(m_whiteBut, &QRadioButton::clicked, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.whiteMove = true;
    });
    connect(m_blackBut, &QRadioButton::clicked, this, [this]() {
        m_errorLabel->clear();
        m_boardParams.whiteMove = false;
    });
}

ChessBoardParams &BoardSetupWindow::getBoardParams()
{
    return m_boardParams;
}
