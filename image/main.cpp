#include "imagewidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageWidget w;
    w.show();

    return a.exec();
}
