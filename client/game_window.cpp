#include "game_window.h"

GameWindow::GameWindow(QWidget *parent)
    : QWidget{parent}
{
    this->resize(727, 717);
    this->setWindowTitle("Chess Online");
    this->setWindowIcon(QIcon(path + "avatar.png"));

    m_board = new ChessBoard(this);
    auto lab = new QLabel("text from");
    lab->setFixedSize(100, 30);

    auto *mainLayout = new SquareVBoxLayout();
    mainLayout->addWidget(m_board);
    mainLayout->addWidget(lab);

    this->setLayout(mainLayout);
}
