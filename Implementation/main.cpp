#include "View/ConnectWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectWindow * w = new ConnectWindow();
    w->show();
    return a.exec();
}
