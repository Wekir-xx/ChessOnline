#include "main_window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SomeConstans::getInstance();
    MainWindow w;
    w.show();

    return a.exec();
}
