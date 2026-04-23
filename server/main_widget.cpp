#include "main_widget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    m_mainLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();
    m_buttonsLayout = new QGridLayout();

    m_timeWidget = new TimeWidget();

    m_logWidget = new QTextEdit();
    m_clearLog = new QPushButton("Clear log");

    m_logWidget->setReadOnly(true);

    m_topLayout->addWidget(m_timeWidget);
    m_topLayout->addStretch();
    m_topLayout->addWidget(m_clearLog);

    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_logWidget);
    m_mainLayout->addLayout(m_buttonsLayout);

    this->setLayout(m_mainLayout);

    connect(m_clearLog, &QPushButton::clicked, this, &MainWidget::clearLog);
}

void MainWidget::addLog(QString log, MESSAGE_TYPE type)
{
    const QString str = "<span style='color:%1; font-size: 13px; font-weight:bold;'>%2</span>";
    QString status;
    switch (type) {
    case MESSAGE_TYPE::INFO:
        status = str.arg("").arg("INFO");
        break;
    case MESSAGE_TYPE::SUCCESS:
        status = str.arg("green").arg("SUCCESS");
        break;
    case MESSAGE_TYPE::ERROR:
        status = str.arg("red").arg("ERROR");
        break;
    }

    m_logWidget->append(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")
                        + " [" + status + "] >> " + log);
}

void MainWidget::clearLog()
{
    m_logWidget->clear();
}
