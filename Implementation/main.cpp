#include "Headers/connectwindow.h"
#include "Headers/mainwindow.h"
#include <QApplication>
#include <QTextEdit>
#include <QTreeView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*ConnectWindow * w = new ConnectWindow();
    w->show();
    w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size()
                                       , a.desktop()->availableGeometry()));*/
    QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QOCI"));
    db->setHostName("aramis.inf.elte.hu");
    db->setDatabaseName("eszakigrid97");
    db->setPort(1521);
    if (db->open("a8uz7t", "fTzg35!"))
    {
        MainWindow * wa = new MainWindow(*db);
        wa->show();
    }
    return a.exec();
}
