#include "widget.h"
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/snake.png"));

    Widget w;
    w.show();
    return a.exec();
}
