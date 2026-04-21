#include "chess_board_widget.h"

#include <QChar>
#include <QIcon>
#include <QPainter>
#include <QSizePolicy>
#include <QTransform>

ChessBoardWidget::ChessBoardWidget(StyleLib *styleLib, QWidget *parent)
    : QWidget(parent)
{
    m_turnBoard = false;
    m_turnSecondPlayer = false;

    m_styleLib = styleLib;
    m_imagesOfPieces.reserve(40);
    m_chessBoardBut.resize(SIDE_SIZE, std::vector<EventButton *>(SIDE_SIZE, nullptr));
    m_otherBoardLab.resize(2, std::vector<QLabel *>(SIDE_SIZE, nullptr));

    m_imagesOfPieces[""] = QIcon();

    m_mainLayout = new QGridLayout();
    m_mainLayout->setSpacing(0);

    QSizePolicy sizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);

    QLabel *empty = new QLabel();
    empty->setSizePolicy(sizePolicy);
    empty->setFixedHeight(FIXED_SIZE_NUMBERS);
    m_mainLayout->addWidget(empty, SIDE_SIZE, 0);

    for (qint8 i = 0; i < SIDE_SIZE; ++i) {
        QLabel *label = new QLabel();
        label->setMinimumHeight(MINIMUM_PIECE_SIZE);
        label->setFixedWidth(FIXED_SIZE_NUMBERS);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString::number(SIDE_SIZE - i));
        m_otherBoardLab[0][i] = label;

        label = new QLabel();
        label->setMinimumWidth(MINIMUM_PIECE_SIZE);
        label->setFixedHeight(FIXED_SIZE_NUMBERS);
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label->setText(QString(QChar('a' + i)));
        m_otherBoardLab[1][i] = label;

        for (qint8 j = 0; j < SIDE_SIZE; ++j) {
            m_chessBoardBut[i][j] = new EventButton();
            m_chessBoardBut[i][j]->setObjectName(QString(QChar('a' + j)) + QString::number(i + 1));
            m_chessBoardBut[i][j]->setSizePolicy(sizePolicy);
            m_chessBoardBut[i][j]->setMinimumSize(MINIMUM_PIECE_SIZE, MINIMUM_PIECE_SIZE);

            this->baseField(i, j);

            connect(m_chessBoardBut[i][j], &EventButton::pressMouseSignal, this, [this, i, j]() {
                emit clickField(m_chessBoardBut[i][j]->objectName());
            });
        }
    }

    this->fillBoard(false);
    this->fillFullIcans();
    this->setLayout(m_mainLayout);

    connect(m_styleLib, &StyleLib::changeIconStyle, this, [this]() {
        if (m_turnSecondPlayer) {
            if (m_turnBoard) {
                this->fillIcan(true, false);
                this->fillIcan(false, true);
            } else {
                this->fillIcan(true, true);
                this->fillIcan(false, false);
            }
        } else {
            this->fillIcan(true, true);
            this->fillIcan(false, true);
        }
    });
}

void ChessBoardWidget::checkField(qint8 i, qint8 j)
{
    m_chessBoardBut[i][j]->setStyleSheet("background-color: #ff3838; border: none;");
}

void ChessBoardWidget::moveField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #B9CA43; border: none;");
    else
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #F5F682; border: none;");
}

void ChessBoardWidget::baseField(qint8 i, qint8 j)
{
    if ((i + j) % 2 == 0)
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #739552; border: none;");
    else
        m_chessBoardBut[i][j]->setStyleSheet("background-color: #EBECD0; border: none;");
}

void ChessBoardWidget::whiteField(qint8 i, qint8 j)
{
    m_chessBoardBut[i][j]->setStyleSheet("background-color: #FFFFFF; border: none;");
}

void ChessBoardWidget::turnBoard()
{
    m_turnBoard ^= true;
    if (m_turnSecondPlayer) {
        this->fillIcan(!m_turnBoard, true);
        this->fillIcan(m_turnBoard, false);
    }

    this->fillBoard(m_turnBoard);
}

void ChessBoardWidget::turnSecondPlayer()
{
    m_turnSecondPlayer ^= true;
    if (m_turnBoard)
        this->fillIcan(true, !m_turnSecondPlayer);
    else
        this->fillIcan(false, !m_turnSecondPlayer);
}

void ChessBoardWidget::fillBoard(bool turnBoard)
{
    if (turnBoard) {
        for (qint8 i = 0; i < SIDE_SIZE; ++i) {
            m_mainLayout->addWidget(m_otherBoardLab[0][7 - i], i, 0);
            m_mainLayout->addWidget(m_otherBoardLab[1][7 - i], 8, i + 1);

            for (qint8 j = 0; j < SIDE_SIZE; ++j)
                m_mainLayout->addWidget(m_chessBoardBut[i][7 - j], i, j + 1);
        }
    } else {
        for (qint8 i = 0; i < SIDE_SIZE; ++i) {
            m_mainLayout->addWidget(m_otherBoardLab[0][i], i, 0);
            m_mainLayout->addWidget(m_otherBoardLab[1][i], 8, i + 1);

            for (qint8 j = 0; j < SIDE_SIZE; ++j)
                m_mainLayout->addWidget(m_chessBoardBut[7 - i][j], i, j + 1);
        }
    }
}

void ChessBoardWidget::fillIcan(bool white, bool up)
{
    const auto &constans = SomeConstans::getInstance();
    const auto &pieceKeys = white ? constans.getPieceKeysWhite() : constans.getPieceKeysBlack();
    std::unordered_map<QString, QPixmap> pixmapOfPieces;

    QTransform transform;
    transform.rotate(180);
    for (const QString &key : pieceKeys) {
        QPixmap pix = QPixmap(m_styleLib->getStyleIcon() + QString("%1.png").arg(key));
        if (up)
            pixmapOfPieces[key] = pix;
        else
            pixmapOfPieces[key] = pix.transformed(transform);
    }

    QSize baseSize = pixmapOfPieces.begin()->second.size();
    QPixmap overlay(QString(GENERAL_PATH) + "beatPiece.png");
    overlay = overlay.scaled(baseSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int xPos = (baseSize.width() - overlay.width()) / 2;
    int yPos = (baseSize.height() - overlay.height()) / 2;

    for (const QString &key : pieceKeys) {
        QPixmap background = pixmapOfPieces[key];

        QPainter painter(&background);
        painter.setOpacity(BEAT_FIELD_OPACITY);
        painter.drawPixmap(xPos, yPos, overlay);

        m_imagesOfPieces[key] = QIcon(pixmapOfPieces[key]);
        m_imagesOfPieces[key + "beatPiece"] = QIcon(background);
    }
}

void ChessBoardWidget::setIcon(qint8 i, qint8 j, QString piece)
{
    m_chessBoardBut[i][j]->setIcon(m_imagesOfPieces[piece]);
}

void ChessBoardWidget::setIconSize(qint8 i, qint8 j, float koef)
{
    m_chessBoardBut[i][j]->setIconSize(m_chessBoardBut[i][j]->size() * koef);
}

bool ChessBoardWidget::getTurnBoard()
{
    return m_turnBoard;
}

bool ChessBoardWidget::getTurnSecondPlayer()
{
    return m_turnSecondPlayer;
}

void ChessBoardWidget::fillFullIcans()
{
    this->fillIcan(true, true);
    this->fillIcan(false, true);

    QPixmap beatFieldPixmap(QString(GENERAL_PATH) + "beatField.png");
    QPixmap transparentPixmap(beatFieldPixmap.size());
    transparentPixmap.fill(Qt::transparent);

    QPainter painter(&transparentPixmap);
    painter.setOpacity(BEAT_FIELD_OPACITY);
    painter.drawPixmap(0, 0, beatFieldPixmap);
    painter.end();

    m_imagesOfPieces["beatField"] = QIcon(transparentPixmap);
}
