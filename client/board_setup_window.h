#ifndef BOARD_SETUP_WINDOW_H
#define BOARD_SETUP_WINDOW_H

#include "src/defines.h"

#include <QButtonGroup>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

class BoardSetupWindow : public QWidget
{
    Q_OBJECT
public:
    explicit BoardSetupWindow(ChessBoardParams &boardParams, QWidget *parent = nullptr);

    ChessBoardParams &getBoardParams();

signals:
    void saveParams();
    void exit();

private:
    ChessBoardParams m_boardParams;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QVBoxLayout *m_colorLayout;
    QVBoxLayout *m_helperLayout;
    QVBoxLayout *m_960Layout;
    QVBoxLayout *m_saveLayout;
    QGridLayout *m_castlingLayout;

    QPushButton *m_exitBut;
    QPushButton *m_saveBut;
    QPushButton *m_resetBut;
    QPushButton *m_reverseBut;
    QPushButton *m_update960But;
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
