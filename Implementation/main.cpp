#include "Headers/connectwindow.h"
#include "Headers/mainwindow.h"
#include <QApplication>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConnectWindow * w = new ConnectWindow();
    w->show();
    return a.exec();
}
