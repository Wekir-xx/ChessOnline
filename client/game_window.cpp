#include "game_window.h"

GameWindow::GameWindow(QWidget *parent)
    : QWidget{parent}
{
    this->resize(727, 717);
    this->setMinimumSize(QSize(405, 400));
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(":/images/src/avatar.png"));

    m_board = new ChessBoard(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_board);

    this->setLayout(mainLayout);
}
