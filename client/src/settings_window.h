#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "style_lib.h"

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(StyleLib *styleLib, QWidget *parent = nullptr);

signals:
    void settingStyles();
    void exit();

private:
    void setStyle();

private:
    StyleLib *m_styleLib;

    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_topLayout;

    QPushButton *m_exitBut;
    QPushButton *m_settingStylesBut;
};

#endif // SETTINGSWINDOW_H
