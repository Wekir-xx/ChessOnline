#ifndef STYLESWINDOW_H
#define STYLESWINDOW_H

#include "src/style_lib.h"
#include "src/chess_board_widget.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QComboBox>

class StylesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StylesWindow(StyleLib *styleLib, QWidget *parent = nullptr);

signals:
    void exit();

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateIcon();
    void updateBoard();
    void updateBoardSize();
    void setStyle();

private:
    StyleLib *m_styleLib;
    ChessBoardWidget *m_chessBoard;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_downLayout;

    QPushButton *m_exitBut;
    QComboBox *m_iconStyle;
    QComboBox *m_boardStyle;
    QComboBox *m_windowStyle;

    std::vector<std::vector<QString>> m_chessFields;
};

#endif // STYLESWINDOW_H
