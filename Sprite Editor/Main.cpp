#include "MainWindow.h"
#include "NewLayer.h"
#include "TransparencyColour.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    NewLayer nl(&w);
    nl.show();

    TransparencyColour tc(&w);
    tc.show();


    return a.exec();
}
