#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"

#include "Headers/constants.h"

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , db(nullptr)
{
	Q_ASSERT(db);
}

MainWindow::MainWindow(QWidget * parent, QSqlDatabase * db) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(db)
{
    ui->setupUi(this);
    /*QWidget *newTab = new QWidget(ui->tabWidget);
    ui->tabWidget->addTab(newTab, tr("name"));
    ui->tabWidget->addTab(new QWidget(ui->tabWidget), tr("name"));*/
    QString dbName = db->databaseName();
    ui->statusBar->showMessage(dbName);
    QList<int> sizes;
	sizes << 150 << 600;
	ui->mainSplitter->setSizes(sizes);
	sizes.clear();
	sizes << 400 << 200;
	ui->splitter->setSizes(sizes);
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
	QTreeWidgetItem * tables = new QTreeWidgetItem(QStringList("Táblák"));
	QTreeWidgetItem * indexes = new QTreeWidgetItem(QStringList("Indexek"));
	QTreeWidgetItem * views = new QTreeWidgetItem(QStringList("Nézetek"));
	QTreeWidgetItem * functions = new QTreeWidgetItem(QStringList("Függvények"));
	items.append(tables);
	items.append(indexes);
	items.append(views);
	items.append(functions);
	fillTableList(tables);
	fillIndexList(indexes);
	fillViewList(views);
	fillFunctionList(functions);
    ui->treeWidget->insertTopLevelItems(0, items);
    ui->treeWidget->setHeaderLabel(db->hostName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_tabWidget_tabCloseRequested(int index)
//{
//    //delete ui->tabWidget->widget(index);
//}


bool MainWindow::fillList(QTreeWidgetItem * list, QSqlQuery * query)
{
	QList<QTreeWidgetItem *> tableItems;
	if (!query->isActive())
	{
		return false;
	}
	while (query->next())
	{
		tableItems.append(new QTreeWidgetItem(QStringList(query->value(0).toString())));
	}
	list->insertChildren(0, tableItems);
	return true;
}

bool MainWindow::fillTableList(QTreeWidgetItem * table)
{
	QSqlQuery query;
	query.exec(queries::GETTABLES);
	return fillList(table, &query);
}

bool MainWindow::fillIndexList(QTreeWidgetItem * index)
{
	QSqlQuery query;
	query.exec(queries::GETINDEXES);
	return fillList(index, &query);
}

bool MainWindow::fillViewList(QTreeWidgetItem * view)
{
	QSqlQuery query;
	query.exec(queries::GETVIEWS);
	return fillList(view, &query);
}

bool MainWindow::fillFunctionList(QTreeWidgetItem * function)
{
	QSqlQuery query;
	query.exec(queries::GETFUNCTIONS);
	return fillList(function, &query);
}

void MainWindow::on_actionKil_p_s_triggered()
{
    this->close();
    this->destroy();
}
