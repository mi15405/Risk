#include "loadplayers.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    LoadPlayers l;
    l.show();

    return a.exec();
}
