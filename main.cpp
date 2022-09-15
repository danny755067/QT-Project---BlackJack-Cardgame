#include "cardpanel.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CardPanel w;
    w.show();

    return a.exec();
}
