#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

#include "Headers/constants.h"

MainWindow::MainWindow(QSqlDatabase& db, QWidget * parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(db)
	, logger(*new QPlainTextEdit)
{
	ui->setupUi(this);

	progressBar = new QProgressBar(ui->statusBar);
	progressBar->setRange(0, 100);
    progressBar->setValue(0);
	progressBar->setMaximumSize(150, 18);

	ui->mainSplitter->setSizes({ 150, 600 });
	ui->splitter->setSizes({ 400, 200 });

    QTextEdit *newTab = new QTextEdit(ui->tWUpper);
    QString dbName = db.hostName();
	ui->tWUpper->addTab(newTab, dbName);

	QTreeWidget * treeWidget = ui->trWLeft;
    treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem * tables = new QTreeWidgetItem(QStringList(queries::TABLES));
    QTreeWidgetItem * indexes = new QTreeWidgetItem(QStringList(queries::INDEXES));
    QTreeWidgetItem * views = new QTreeWidgetItem(QStringList(queries::VIEWS));
    QTreeWidgetItem * functions = new QTreeWidgetItem(QStringList(queries::FUNCTIONS));
	items.append(tables);
	items.append(indexes);
	items.append(views);
    items.append(functions);
    treeWidget->insertTopLevelItems(0, items);
    treeWidget->setHeaderLabel(dbName);
    ui->statusBar->addPermanentWidget(progressBar, 0);

	ui->tWLower->addTab(&logger, "Adatbázis log");
	logger.setReadOnly(true);
	logger.appendPlainText(QString("[%1] Sikeresen csatlakozva az adatbázishoz!").arg(QTime::currentTime().toString()));

	ui->tWLower->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete progressBar;
}


bool MainWindow::fillList(QTreeWidgetItem * list, QSqlQuery& query, int tableSize)
{
    // TODO: seperate child then replace original in tree
    if (!query.isActive())
    {
		return false;
    }
    for (int i = 0; i < tableSize; ++i)
    {
        query.next();
        list->insertChild(0, new QTreeWidgetItem(
                              QStringList(query.value(0).toString())
                              )
                          );
        progressBar->setValue((i + 1.0) / tableSize * 100);
    }
	progressBar->setValue(0);
	return true;
}

bool MainWindow::fillTableList(QTreeWidgetItem * table)
{
    QSqlQuery query = db.exec(queries::GET_TABLES_COUNT);
    query.next();
    int tableSize = query.value(0).toInt();
    query = db.exec(queries::GET_TABLES);;
    return fillList(table, query, tableSize);
}

bool MainWindow::fillIndexList(QTreeWidgetItem * index)
{
	QSqlQuery query;
	query.exec(queries::GET_INDEXES);
    return fillList(index, query);
}

bool MainWindow::fillViewList(QTreeWidgetItem * view)
{
	QSqlQuery query;
	query.exec(queries::GET_VIEWS);
    return fillList(view, query);
}

bool MainWindow::fillFunctionList(QTreeWidgetItem * function)
{
	QSqlQuery query;
	query.exec(queries::GET_FUNCTIONS);
    return fillList(function, query);
}

void MainWindow::on_actionKil_p_s_triggered()
{
    this->close();
    this->destroy();
}

void MainWindow::on_trWLeft_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	QElapsedTimer timer;
	if (item->childCount() < 1)
	{
		bool success;
		int elapsedTime;
		if (queries::TABLES == item->text(0))
		{
			timer.start();
			success = fillTableList(item);
			elapsedTime = timer.elapsed();
		}
		else if (queries::INDEXES == item->text(0))
		{
			timer.start();
			success = fillIndexList(item);
			elapsedTime = timer.elapsed();
		}
		else if (queries::VIEWS == item->text(0))
		{
			timer.start();
			success = fillViewList(item);
			elapsedTime = timer.elapsed();
		}
		else if (queries::FUNCTIONS == item->text(0))
		{
			timer.start();
			success = fillFunctionList(item);
			elapsedTime = timer.elapsed();
		}
		if (success)
		{
			logger.appendPlainText(QString("[%1] Adatbázisban található objektumok sikeresen lekérdezve %2 ms alatt.").arg(QTime::currentTime().toString()).arg(elapsedTime));
		}
		else
		{
			logger.appendPlainText("Hiba történt a lekérdezés végrehajtása közben!");
		}
	}
}

void MainWindow::on_tWUpper_tabCloseRequested(int index)
{
	delete ui->tWUpper->widget(index);
}

void MainWindow::on_tWLower_tabCloseRequested(int index)
{
	delete ui->tWLower->widget(index);
}
