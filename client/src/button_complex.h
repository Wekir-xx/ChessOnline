#ifndef BUTTON_COMPLEX_H
#define BUTTON_COMPLEX_H

#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include <vector>

class ButtonComplex : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonComplex(QWidget *parent = nullptr);

    void setButtons(std::vector<std::string> nameButtons);
    void useButton(size_t num);

signals:
    void selectButtonSignals(size_t num);

private:
    void unUseButton();

private:
    QHBoxLayout *m_layout;

    std::vector<QPushButton *> m_buttons;
    size_t m_numUseButton;
};

#endif // BUTTON_COMPLEX_H
