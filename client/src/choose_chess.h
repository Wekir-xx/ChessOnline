#ifndef CHOOSE_CHESS_H
#define CHOOSE_CHESS_H

#include "src/style_lib.h"

#include <QGridLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QShowEvent>
#include <QWidget>

class ChooseChess : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseChess(StyleLib *styleLib, QWidget *parent = nullptr);

signals:
    void selectPiece(QString piece);

protected:
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateIcon();
    void updateSize();

private:
    StyleLib *m_styleLib;
    SomeConstans *m_constans;

    QGridLayout *m_mainLayout;

    std::vector<QPushButton *> m_buttons;
    size_t m_idUseBut;
};

#endif // CHOOSE_CHESS_H
