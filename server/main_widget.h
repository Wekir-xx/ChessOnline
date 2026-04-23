#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "time_widget.h"
#include "defines.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QGridLayout>
#include <QPushButton>

class MainWidget : public QWidget
{
public:
    explicit MainWidget(QWidget *parent = nullptr);

private:
    void addLog(QString log, MESSAGE_TYPE type);
    void clearLog();

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;
    QGridLayout *m_buttonsLayout;

    TimeWidget *m_timeWidget;

    QTextEdit *m_logWidget;
    QPushButton *m_clearLog;
};

#endif // MAINWIDGET_H
