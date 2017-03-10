#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QSqlDatabase& db, QWidget * parent) :
    QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(db)
	, logger(*new DBLogger)
{
	ui->setupUi(this);

	progressBar = new QProgressBar(ui->statusBar);
	progressBar->setRange(0, 100);
    progressBar->setValue(0);
	progressBar->setMaximumSize(150, 18);

	ui->mainSplitter->setSizes({ 150, 600 });
	ui->splitter->setSizes({ 400, 200 });

    queries = new QTextEdit(ui->tWUpper);
	queries->textCursor().insertHtml("Select * FROM emp;");
	queries->setFont(QFont("Segoe UI", 11));
	SqlHighlighter * highLighter = new SqlHighlighter();
	highLighter->setDocument(queries->document());
    QString dbName = db.hostName();
	ui->tWUpper->addTab(queries, dbName);

	QTreeWidget * treeWidget = ui->trWLeft;
    treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
	QTreeWidgetItem * tables	= new QTreeWidgetItem(QStringList(queries::TABLES));
    QTreeWidgetItem * indexes	= new QTreeWidgetItem(QStringList(queries::INDEXES));
    QTreeWidgetItem * views		= new QTreeWidgetItem(QStringList(queries::VIEWS));
    QTreeWidgetItem * functions = new QTreeWidgetItem(QStringList(queries::FUNCTIONS));
	items.append(tables);
	items.append(indexes);
	items.append(views);
    items.append(functions);
    treeWidget->insertTopLevelItems(0, items);
    treeWidget->setHeaderLabel(dbName);
    ui->statusBar->addPermanentWidget(progressBar, 0);

	ui->tWLower->addTab(&logger, "Adatbázis log");
	logger.appendPlainText("Sikeresen csatlakozva az adatbázishoz!");

	ui->tWUpper->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
	ui->tWLower->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);

	QShortcut * shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), queries);
	QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(executeQuery()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete progressBar;
}


bool MainWindow::fillList(QTreeWidgetItem * list, const QString& queryToExecute, const QString& queryCount)
{
	QSqlDatabase::database().transaction();
	QSqlQuery query = db.exec(queryCount);
	query.next();
	int tableSize = query.value(0).toInt();
	query = db.exec(queryToExecute);
    if (!query.isActive())
    {
		return false;
    }
    for (int i = 0; i < tableSize && query.next(); ++i)
    {
        list->insertChild(i, new QTreeWidgetItem(
                              QStringList(query.value(0).toString())
                              )
                          );
        progressBar->setValue((i + 1.0) / tableSize * 100);
    }
	progressBar->setValue(0);
	QSqlDatabase::database().commit();
	return true;
}

bool MainWindow::fillTableList(QTreeWidgetItem * table)
{
    return fillList(table, queries::GET_TABLES, queries::GET_TABLES_COUNT);
}

bool MainWindow::fillIndexList(QTreeWidgetItem * index)
{
	return fillList(index, queries::GET_INDEXES, queries::GET_INDEXES_COUNT);
}

bool MainWindow::fillViewList(QTreeWidgetItem * view)
{
	return fillList(view, queries::GET_VIEWS, queries::GET_VIEWS_COUNT);
}

bool MainWindow::fillFunctionList(QTreeWidgetItem * function)
{
	return fillList(function, queries::GET_FUNCTIONS, queries::GET_FUNCTIONS_COUNT);
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
		std::function<bool()> toExecute;
		if (queries::TABLES == item->text(0))
		{
			toExecute = [&] { return fillTableList(item); };
		}
		else if (queries::INDEXES == item->text(0))
		{
			toExecute = [&] { return fillIndexList(item); };
		}
		else if (queries::VIEWS == item->text(0))
		{
			toExecute = [&] { return fillViewList(item); };
		}
		else if (queries::FUNCTIONS == item->text(0))
		{
			toExecute = [&] { return fillFunctionList(item); };
		}
		logger.logWithTime("Adatbázisban található objektumok sikeresen lekérdezve.", "Hiba történt a lekérdezés végrehajtása közben!", toExecute);
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

/*
 * Finds and executes query "near" the cursor in the current document.
 * It selects every text until EOL, from previous ;.
 *
 */
void MainWindow::executeQuery()
{
	QTextCursor cursor(queries->textCursor());
	cursor.beginEditBlock();
	cursor.clearSelection();
	cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
	while (!cursor.selectedText().contains(";") && cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor));
	cursor.clearSelection();
	cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);
	while (cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor) && cursor.selectedText().left(1) != ";");
	if (!cursor.atStart())
		cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
	cursor.endEditBlock();

	QString query = cursor.selectedText().simplified();
	QSqlQuery * q = new QSqlQuery(db);
	query = query.remove(QRegExp(";"));
	std::function<bool()> toExecute = [&] { if (db.isValid()) { q->exec(query); }
											return q->isActive(); };

	if ('S' == query.begin()->toUpper())
	{
		bool success = logger.logWithTime("Lekérdezés sikeresen végrehajtva!", "Lekérdezés sikertelen.", toExecute);
		if (success)
		{
			QSqlQueryModel* model = new QSqlQueryModel;
			model->setQuery(*q);
			QTableView *view = new QTableView(ui->tWLower);
			view->setModel(model);
			view->show();
			ui->tWLower->addTab(view, "Lekérdezés eredménye");
			ui->tWLower->setCurrentWidget(view);
		}
	}
	else if ('D' == query.begin()->toUpper())
	{
		logger.logWithTime("Törlés sikeresen végrehajtva.", "Törlés sikertelen.", toExecute);
	}
	else if ('U' == query.begin()->toUpper())
	{
		logger.logWithTime("Frissítés sikeresen végrehajtva.", "Frissítés sikertelen.", toExecute);
	}
}
