#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "db_interface.h"
#include "time_widget.h"
#include "defines.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
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
    QVBoxLayout *m_buttonsLayout;
    QHBoxLayout *m_topLayout;

    DBInterface *m_dbInterface;
    TimeWidget *m_timeWidget;

    QTextEdit *m_logWidget;
    QPushButton *m_clearLogBut;
    QPushButton *m_workServerBut;
    QPushButton *m_testFunctionBut;
};

#endif // MAINWIDGET_H
