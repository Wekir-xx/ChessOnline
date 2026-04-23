#ifndef BOARD_SETUP_WINDOW_H
#define BOARD_SETUP_WINDOW_H

#include "chess_game.h"
#include "src/board_hlayout2.h"
#include "src/board_vlayout2.h"
#include "src/chess_board_widget.h"
#include "src/choose_chess.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QWidget>

class BoardSetupWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BoardSetupWindow(StyleLib *styleLib, ChessBoardParams &boardParams, QWidget *parent = nullptr);

    void startGame();

    ChessBoardParams &getBoardParams();

signals:
    void saveParams();
    void exit();

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateIcon();
    void updateIconSize();
    void clearBoard();

    void checkSave();

    void setStyle();

private:
    StyleLib *m_styleLib;
    ChessGame m_game;
    ChessBoardWidget *m_chessBoard;
    ChooseChess *m_chooseChess;

    ChessBoardParams m_boardParams;
    ChessBoardParams m_copyBoardParams;
    QString m_piece;

    BoardVLayout2 *m_mainLayout;
    BoardHLayout2 *m_boardPieceLayout;
    QVBoxLayout *m_topLayout;
    QHBoxLayout *m_topLayoutPart;
    QVBoxLayout *m_colorLayout;
    QVBoxLayout *m_turnLayout;
    QVBoxLayout *m_clearResetLayout;
    QVBoxLayout *m_saveResetLayout;
    QGridLayout *m_castlingLayout;

    QPushButton *m_exitBut;
    QPushButton *m_saveBut;
    QPushButton *m_turnBoardBut;
    QPushButton *m_turnPlayerBut;
    QPushButton *m_resetBoardBut;
    QPushButton *m_resetPosBut;
    QPushButton *m_clearBut;
    QCheckBox *m_whiteCastlingBut1;
    QCheckBox *m_whiteCastlingBut2;
    QCheckBox *m_blackCastlingBut1;
    QCheckBox *m_blackCastlingBut2;
    QCheckBox *m_960But;
    QRadioButton *m_whiteBut;
    QRadioButton *m_blackBut;
    QLabel *m_whiteCastlingLabel;
    QLabel *m_blackCastlingLabel;
    QLabel *m_errorLabel;
};

#endif // BOARD_SETUP_WINDOW_H
