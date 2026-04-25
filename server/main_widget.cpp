#include "main_widget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget{parent}
{
    m_mainLayout = new QVBoxLayout();
    m_buttonsLayout = new QVBoxLayout();
    m_topLayout = new QHBoxLayout();

    m_dbInterface = new DBInterface(this);
    m_timeWidget = new TimeWidget();

    m_logWidget = new QTextEdit();
    m_clearLogBut = new QPushButton("Clear log");
    m_workServerBut = new QPushButton("Start");
    m_testFunctionBut = new QPushButton("Test Function");

    m_logWidget->setReadOnly(true);

    m_buttonsLayout->addWidget(m_workServerBut);
    m_buttonsLayout->addWidget(m_testFunctionBut);

    m_topLayout->addWidget(m_timeWidget);
    m_topLayout->addStretch();
    m_topLayout->addWidget(m_clearLogBut);

    m_mainLayout->addLayout(m_topLayout);
    m_mainLayout->addWidget(m_logWidget);
    m_mainLayout->addLayout(m_buttonsLayout);

    this->setLayout(m_mainLayout);

    connect(m_clearLogBut, &QPushButton::clicked, this, &MainWidget::clearLog);
    connect(m_workServerBut, &QPushButton::clicked, this, [this]() {
        if (m_workServerBut->text() == "Start") {
            addLog("Start server", MESSAGE_TYPE::INFO);
            m_workServerBut->setText("Stop");
            m_timeWidget->startTime();
        } else {
            addLog("Stop server", MESSAGE_TYPE::INFO);
            m_workServerBut->setText("Start");
            m_timeWidget->stopTime();
        }
    });
    connect(m_testFunctionBut, &QPushButton::clicked, this, [this]() {
        this->addLog("test work", MESSAGE_TYPE::INFO);
    });

    addLog("Server has been initialized", MESSAGE_TYPE::SUCCESS);
}

void MainWidget::addLog(QString log, MESSAGE_TYPE type)
{
    const QString str = "<span style='color: #%1; font-size: 13px; font-weight:bold;'>%2</span>";
    QString status;
    switch (type) {
    case MESSAGE_TYPE::INFO:
        status = str.arg("000", "INFO");
        break;
    case MESSAGE_TYPE::SUCCESS:
        status = str.arg("37cf00", "SUCCESS");
        break;
    case MESSAGE_TYPE::WARNING:
        status = str.arg("c6c900", "WARNING");
        break;
    case MESSAGE_TYPE::ERROR:
        status = str.arg("c00", "ERROR");
        break;
    }

    m_logWidget->append(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss")
                        + " [" + status + "] >> " + log);
}

void MainWidget::clearLog()
{
    m_logWidget->clear();
}
