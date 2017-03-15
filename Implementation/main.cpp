#include "Headers/connectwindow.h"
#include "Headers/mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ConnectWindow * w = new ConnectWindow();
	w->show();
	w->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, w->size()
										, a.desktop()->availableGeometry()));
	return a.exec();
}
