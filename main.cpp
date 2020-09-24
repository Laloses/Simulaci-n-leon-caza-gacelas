#include "pradera.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pradera w;
    w.show();

    return a.exec();
}
