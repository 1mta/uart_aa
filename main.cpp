#include "uart_ass.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uart_ass w;
    w.show();
    return a.exec();
}
