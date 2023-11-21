#include "TwixtGUIQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TwixtGUIQt w;
    w.show();
    return a.exec();
}
