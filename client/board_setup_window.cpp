#include "board_setup_window.h"

#include <QPainter>
#include <QPixmap>

BoardSetupWindow::BoardSetupWindow(StyleLib *styleLib, ChessBoardParams &boardParams, QWidget *parent)
    : QWidget{parent}
    , m_boardParams{boardParams}
    , m_copyBoardParams{boardParams}
{
    QString fontSize = styleLib->getFontSizeStyle();

    m_chessBoard = new ChessBoardWidget(styleLib);
    m_chooseChess = new ChooseChess(styleLib);

    m_mainLayout = new BoardVLayout2();
    m_boardPieceLayout = new BoardHLayout2();
    m_topLayout = new QVBoxLayout();
    m_topLayoutPart = new QHBoxLayout();
    m_colorLayout = new QVBoxLayout();
    m_turnLayout = new QVBoxLayout();
    m_clearResetLayout = new QVBoxLayout();
    m_saveResetLayout = new QVBoxLayout();
    m_castlingLayout = new QGridLayout();

    m_exitBut = new QPushButton();
    m_saveBut = new QPushButton("Save");
    m_turnBoardBut = new QPushButton("Turn board");
    m_turnPlayerBut = new QPushButton("Turn player");
    m_resetBoardBut = new QPushButton("Reset update");
    m_resetPosBut = new QPushButton();
    m_clearBut = new QPushButton("Clear board");
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
    m_turnBoardBut->setStyleSheet(fontSize);
    m_turnPlayerBut->setStyleSheet(fontSize);
    m_resetBoardBut->setStyleSheet(fontSize);
    m_resetPosBut->setStyleSheet(fontSize);
    m_clearBut->setStyleSheet(fontSize);
    m_960But->setStyleSheet(fontSize);
    m_whiteCastlingLabel->setStyleSheet(fontSize);
    m_blackCastlingLabel->setStyleSheet(fontSize);
    m_errorLabel->setStyleSheet("color: red; font-size: 18px;");
    m_errorLabel->setAlignment(Qt::AlignCenter);

    m_whiteBut->setFixedWidth(50);
    m_blackBut->setFixedWidth(50);

    if (m_boardParams.whiteMove)
        m_whiteBut->setChecked(true);
    else
        m_blackBut->setChecked(true);

    if (m_boardParams.chess960)
        m_resetPosBut->setText("New pos");
    else
        m_resetPosBut->setText("Reset pos");

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
    m_exitBut->setIcon(QIcon(QString(GENERAL_PATH) + "exit.png"));

    m_whiteBut->setIconSize(pixmapWhite.size());
    m_blackBut->setIconSize(pixmapBlack.size());
    m_exitBut->setIconSize(m_exitBut->size());

    m_colorLayout->addWidget(m_whiteBut);
    m_colorLayout->addWidget(m_blackBut);

    m_turnLayout->addWidget(m_turnBoardBut);
    m_turnLayout->addWidget(m_turnPlayerBut);

    m_clearResetLayout->addWidget(m_clearBut);
    m_clearResetLayout->addWidget(m_resetPosBut);

    m_castlingLayout->setSpacing(0);
    m_castlingLayout->addWidget(m_whiteCastlingLabel, 0, 0);
    m_castlingLayout->addWidget(m_whiteCastlingBut1, 0, 1);
    m_castlingLayout->addWidget(m_whiteCastlingBut2, 0, 2);
    m_castlingLayout->addWidget(m_blackCastlingLabel, 1, 0);
    m_castlingLayout->addWidget(m_blackCastlingBut1, 1, 1);
    m_castlingLayout->addWidget(m_blackCastlingBut2, 1, 2);

    m_saveResetLayout->addWidget(m_saveBut);
    m_saveResetLayout->addWidget(m_resetBoardBut);

    m_topLayoutPart->addWidget(m_exitBut);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addLayout(m_colorLayout);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addWidget(m_960But);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addLayout(m_turnLayout);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addLayout(m_clearResetLayout);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addLayout(m_castlingLayout);
    m_topLayoutPart->addStretch();
    m_topLayoutPart->addLayout(m_saveResetLayout);

    m_topLayout->addLayout(m_topLayoutPart);
    m_topLayout->addWidget(m_errorLabel);

    m_boardPieceLayout->addWidget(m_chessBoard);
    m_boardPieceLayout->addWidget(m_chooseChess);

    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addLayout(m_boardPieceLayout);

    this->setLayout(m_mainLayout);

    this->updateBoardIcon();

    connect(m_exitBut, &QPushButton::clicked, this, [this]() { emit exit(); });
    connect(m_saveBut, &QPushButton::clicked, this, &BoardSetupWindow::checkSave);
    connect(m_turnBoardBut, &QPushButton::clicked, this, [this]() {
        m_chessBoard->turnBoard();

        if (m_chessBoard->getTurnSecondPlayer())
            this->updateBoardIcon();
    });
    connect(m_turnPlayerBut, &QPushButton::clicked, this, [this]() {
        m_chessBoard->turnSecondPlayer();
        this->updateBoardIcon();
    });
    connect(m_resetBoardBut, &QPushButton::clicked, this, [this]() {
        m_errorLabel->clear();

        m_copyBoardParams.chessFields = m_boardParams.chessFields;
        this->updateBoardIcon();
    });
    connect(m_resetPosBut, &QPushButton::clicked, this, [this]() {
        m_errorLabel->clear();

        if (m_copyBoardParams.chess960)
            SomeConstans::fill960ChessField(m_copyBoardParams.chessFields);
        else
            SomeConstans::fillStandartChessField(m_copyBoardParams.chessFields);

        this->updateBoardIcon();
    });
    connect(m_clearBut, &QPushButton::clicked, this, [this]() {
        m_errorLabel->clear();

        for (qint8 i = 0; i < SIDE_SIZE; ++i)
            for (qint8 j = 0; j < SIDE_SIZE; ++j)
                m_copyBoardParams.chessFields[i][j] = "";

        this->updateBoardIcon();
    });
    connect(m_whiteCastlingBut1, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.castling.first.first ^= true;
    });
    connect(m_whiteCastlingBut2, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.castling.first.second ^= true;
    });
    connect(m_blackCastlingBut1, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.castling.second.first ^= true;
    });
    connect(m_blackCastlingBut2, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.castling.second.second ^= true;
    });
    connect(m_960But, &QCheckBox::checkStateChanged, this, [this]() {
        m_errorLabel->clear();

        m_copyBoardParams.chess960 ^= true;
        if (m_copyBoardParams.chess960)
            m_resetPosBut->setText("New pos");
        else
            m_resetPosBut->setText("Reset pos");
    });
    connect(m_whiteBut, &QRadioButton::clicked, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.whiteMove = true;
    });
    connect(m_blackBut, &QRadioButton::clicked, this, [this]() {
        m_errorLabel->clear();
        m_copyBoardParams.whiteMove = false;
    });
    connect(m_chessBoard, &ChessBoardWidget::clickField, this, [this](QString field) {
        m_errorLabel->clear();

        qint8 i = field[1].digitValue() - 1;
        qint8 j = field[0].unicode() - 'a';

        if (m_copyBoardParams.chessFields[i][j] == m_piece)
            m_copyBoardParams.chessFields[i][j].clear();
        else
            m_copyBoardParams.chessFields[i][j] = m_piece;

        m_chessBoard->setIcon(i, j, m_copyBoardParams.chessFields[i][j]);
    });
    connect(m_chooseChess, &ChooseChess::selectPiece, this, [this](QString piece) {
        if (m_piece == piece)
            m_piece.clear();
        else
            m_piece = piece;
    });
}

ChessBoardParams &BoardSetupWindow::getBoardParams()
{
    return m_boardParams;
}

void BoardSetupWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    this->updateBoardSize();
}

void BoardSetupWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->updateBoardSize();
}

void BoardSetupWindow::updateBoardIcon()
{
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->setIcon(i, j, m_copyBoardParams.chessFields[i][j]);
}

void BoardSetupWindow::updateBoardSize()
{
    for (qint8 i = 0; i < SIDE_SIZE; ++i)
        for (qint8 j = 0; j < SIDE_SIZE; ++j)
            m_chessBoard->setIconSize(i, j);
}

void BoardSetupWindow::checkSave()
{
    if (!m_whiteBut->isChecked() && !m_blackBut->isChecked()) {
        m_errorLabel->setText("First move color not selected");
        return;
    }

    std::vector<std::pair<qint8, qint8>> whiteKings;
    std::vector<std::pair<qint8, qint8>> blackKings;
    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        for (qint8 j = 0; j < SIDE_SIZE; ++j) {
            if (m_copyBoardParams.chessFields[i][j] == "wK")
                whiteKings.push_back({i, j});
            else if (m_copyBoardParams.chessFields[i][j] == "bK")
                blackKings.push_back({i, j});
        }
    }

    if (whiteKings.size() != 1) {
        m_errorLabel->setText("Must be one white king");
        return;
    }
    if (blackKings.size() != 1) {
        m_errorLabel->setText("Must be one black king");
        return;
    }

    const auto &castling = m_copyBoardParams.castling;

    if (castling.first.first && whiteKings[0].first != 0) {
        m_errorLabel->setText("White castling 0-0-0 not impossible, king not stay on row 1");
        return;
    }
    if (castling.first.second && whiteKings[0].first != 0) {
        m_errorLabel->setText("White castling 0-0 not impossible, king not stay on row 1");
        return;
    }
    if (castling.second.first && blackKings[0].first != 7) {
        m_errorLabel->setText("Black castling 0-0-0 not impossible, king not stay on row 7");
        return;
    }
    if (castling.second.second && blackKings[0].first != 7) {
        m_errorLabel->setText("Black castling 0-0 not impossible, king not stay on row 7");
        return;
    }

    if (m_copyBoardParams.chess960) {
        if (castling.first.first) {
            bool flag = true;

            for (qint8 i = 0; i < whiteKings[0].second; ++i)
                if (m_copyBoardParams.chessFields[0][i] == "wR")
                    flag = false;

            if (flag) {
                m_errorLabel->setText("White castling 0-0-0 not impossible, rook not stay on row 1 and left of king");
                return;
            }
        }
        if (castling.first.second) {
            bool flag = true;

            for (qint8 i = whiteKings[0].second + 1; i < SIDE_SIZE; ++i)
                if (m_copyBoardParams.chessFields[0][i] == "wR")
                    flag = false;

            if (flag) {
                m_errorLabel->setText("White castling 0-0 not impossible, rook not stay on row 1 and right of king");
                return;
            }
        }
        if (castling.second.first) {
            bool flag = true;

            for (qint8 i = 0; i < blackKings[0].second; ++i)
                if (m_copyBoardParams.chessFields[7][i] == "bR")
                    flag = false;

            if (flag) {
                m_errorLabel->setText("Black castling 0-0-0 not impossible, rook not stay on row 7 and left of king");
                return;
            }
        }
        if (castling.second.second) {
            bool flag = true;

            for (qint8 i = blackKings[0].second + 1; i < SIDE_SIZE; ++i)
                if (m_copyBoardParams.chessFields[7][i] == "bR")
                    flag = false;

            if (flag) {
                m_errorLabel->setText("Black castling 0-0 not impossible, rook not stay on row 7 and right of king");
                return;
            }
        }
    } else {
        if (castling.first.first && m_copyBoardParams.chessFields[0][0] != "wR") {
            m_errorLabel->setText("White castling 0-0-0 not impossible, rook not stay on row 1 col a");
            return;
        }
        if (castling.first.second && m_copyBoardParams.chessFields[0][7] != "wR") {
            m_errorLabel->setText("White castling 0-0 not impossible, rook not stay on row 1 col h");
            return;
        }
        if (castling.second.first && m_copyBoardParams.chessFields[7][0] != "bR") {
            m_errorLabel->setText("Black castling 0-0-0 not impossible, rook not stay on row 7 col a");
            return;
        }
        if (castling.second.second && m_copyBoardParams.chessFields[7][7] != "bR") {
            m_errorLabel->setText("Black castling 0-0 not impossible, rook not stay on row 7 col h");
            return;
        }
    }

    m_boardParams = m_copyBoardParams;
    emit saveParams();
}
