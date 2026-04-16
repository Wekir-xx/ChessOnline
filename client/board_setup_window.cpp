#include "board_setup_window.h"

BoardSetupWindow::BoardSetupWindow(ChessBoardParams &boardParams, QWidget *parent)
    : QWidget{parent}
    , m_boardParams{boardParams}
{}

ChessBoardParams &BoardSetupWindow::getBoardParams()
{
    return m_boardParams;
}
