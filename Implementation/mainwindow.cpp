#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QSqlDatabase& database, QWidget * parent) :
	QMainWindow(parent)
  , logger(*new DBLogger)
	, ui(new Ui::MainWindow)
    , db(database)
{
    ui->setupUi(this);

	progressBar = new QProgressBar(ui->statusBar);
	progressBar->setRange(0, 100);
	progressBar->setValue(0);
	progressBar->setMaximumSize(150, 18);
	logger.setProgressBar(progressBar);

	ui->mainSplitter->setSizes({ 150, 600 });
	ui->splitter->setSizes({ 400, 200 });

    queries = new SqlEditor(ui->tWUpper);
	queries->setFont(QFont("Segoe UI", 11));
	SqlHighlighter * highLighter = new SqlHighlighter();
	highLighter->setDocument(queries->document());
	QString dbName = db.hostName();
	ui->tWUpper->addTab(queries, dbName);
    queries->setText("MAKE CHART PIECHART ( SELECT department_id, COUNT(*) FROM employees GROUP BY department_id );");

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

    registerShortcuts();
}


bool MainWindow::fillList(QTreeWidgetItem* list, const QString& queryToExecute) noexcept
{
    //setUpdatesEnabled(false);
    QList<QTreeWidgetItem*> items;
    //QFuture<void> aSyncCall = QtConcurrent::run([&] {
    QSqlQuery query = db.exec(queryToExecute);
    if (!query.isActive())
	{
        // return false;
	}
    //QFuture<void> future = QtConcurrent::run([&] {
	while (query.next())
	{
        items.append(new QTreeWidgetItem(
                         QStringList(query.value(0).toString())));
        //);
    }
    //} });
    std::cerr << "kész";
    //setUpdatesEnabled(true);
    //});
    //list->addChildren(items);
    //future.waitForFinished();
    list->addChildren(items);
    //QtConcurrent::run(list, &QTreeWidgetItem::addChildren, items);
    //QFuture<void> futura = QtConcurrent::run(list, &QTreeWidgetItem::addChildren, items);
    return true;
}

void MainWindow::executeSelection()
{
}

void MainWindow::registerShortcuts()
{
    QShortcut * shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), queries);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(executeQuery()));

    shortcut = new QShortcut(QKeySequence(Qt::Key_F4), queries);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(showExecutionPlan()));

    shortcut = new QShortcut(QKeySequence(Qt::Key_F3), queries);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(executeSelection()));
}

void MainWindow::on_trWLeft_itemDoubleClicked(QTreeWidgetItem* item, int /*column*/)
{
    qDeleteAll(item->takeChildren());
    std::function<bool(QString&)> toExecute;
	if (queries::TABLES == item->text(0))
	{
        toExecute = [&] (QString& /*failMessage*/) { return fillTableList(item); };
	}
	else if (queries::INDEXES == item->text(0))
	{
        toExecute = [&] (QString& /*failMessage*/) { return fillIndexList(item); };
	}
	else if (queries::VIEWS == item->text(0))
	{
        toExecute = [&] (QString& /*failMessage*/) { return fillViewList(item); };
	}
	else if (queries::FUNCTIONS == item->text(0))
	{
        toExecute = [&] (QString& /*failMessage*/) { return fillFunctionList(item); };
	}
    else
    {
        on_actionMegtekint_s_triggered();
        return;
    }

    std::thread t([&] {
        logger.logWithTime("Adatbázisban található objektumok sikeresen lekérdezve."
                           , "Hiba történt a lekérdezés végrehajtása közben!", toExecute);
    });
    t.join();
}

void MainWindow::showExecutionPlan()
{
    std::function<bool(QString&)> toExecute = [&] (QString& failMessage) {
		QSqlQuery * q = new QSqlQuery(db);
		QString query = queries->extractQuery();
		QString explainPlan = QString("EXPLAIN PLAN SET STATEMENT_ID = 'temp1' FOR %1").arg(query);
		bool success = q->exec(explainPlan);
		if (success)
		{
            //success = q->exec("SELECT cardinality \"Rows\", \
				lpad(' ',level-1)||operation||' '||options||' '||object_name \"Plan\" \
				FROM PLAN_TABLE \
				CONNECT BY prior id = parent_id \
				AND prior statement_id = statement_id \
				START WITH id = 0 \
				AND statement_id = 'temp1' \
                ORDER BY id");
             success =     q->exec("SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY())");
			if (success)
			{
				QSqlQueryModel* model = new QSqlQueryModel;
				model->setQuery(*q);
				QTableView *view = new QTableView(ui->tWLower);
				view->setModel(model);
                view->horizontalHeader()->setStretchLastSection(true);
				view->show();
				ui->tWLower->addTab(view, "Lekérdezési terv");
				ui->tWLower->setCurrentWidget(view);
				success = q->exec("DELETE FROM PLAN_TABLE WHERE statement_id = 'temp1'");
			}
		}
        failMessage = q->lastError().text();
		return success;
    };
    logger.logWithTime("Lekérdezési terv sikeresen elkészült!", "Lekérdezési terv létrehozása sikertelen.", toExecute);
}

/*void MainWindow::executeSelection()
{
    QString query = queries->textCursor().selectedText();
    executeString(query);
}*/

void MainWindow::executeString(const QString& query)
{

    QSqlQuery * q = new QSqlQuery(db);
    std::function<bool(QString&)> toExecute = [&] (QString& failMessage) {
            q->exec(query);
            failMessage = q->lastError().text();
            return q->isActive(); };

    QStringList* words = new QStringList(query.split(' '));
    for (auto& i : *words)
    {
        i = i.toUpper();
    }
    if ("SELECT" == words->at(0))
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
    else if ("MAKE" == words->at(0))
    {
        if ("CHART" == words->at(1))
        {
            QString message;
            QT_CHARTS_USE_NAMESPACE
            QChart* chart = new QChart();
            bool success;
            toExecute = [&] (QString& msg)
            {
                success = queries->makeChart(msg
                                   , chart
                                   , words
                                   , query
                                   , q);
                message = msg;
                return success;
            };

            logger.logWithTime("Diagram sikeresen létrehozva!", "Diagram létrehozása közben hiba történt.", toExecute);

            if (success)
            {
                QChartView *chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);
                ui->tWUpper->addTab(chartView, "Diagram eredménye");
                ui->tWUpper->setCurrentWidget(chartView);
            }
        }
    }
    else
    {
        logger.logWithTime("Művelet sikeresen végrehajtva.", "Művelet sikertelen.", toExecute);
    }
}

void MainWindow::on_actionT_rl_s_triggered()
{
	QTreeWidget* tw = ui->trWLeft;
	if (tw->selectedItems().size() > 0)
	{
		QTreeWidgetItem* child = tw->selectedItems().at(0);
		QString parent = child->parent()->text(0);
        std::function<bool(QString&)> toExecute = [&](QString& failMessage) {
            bool success = false;
            if (queries::TABLES == parent)
            {
                success = db.exec(QString("DROP TABLE %1").arg(child->text(0))).isActive();
            }
            else if (queries::INDEXES == parent)
            {
                success = db.exec(QString("DROP INDEX %1").arg(child->text(0))).isActive();
            }
            else if (queries::VIEWS == parent)
            {
                success = db.exec(QString("DROP VIEW %1").arg(child->text(0))).isActive();
            }
            else if (queries::FUNCTIONS == parent)
            {
                success = db.exec(QString("DROP FUNCTION %1").arg(child->text(0))).isActive();
            }
            failMessage = db.lastError().text();
            return success;
        };
        logger.logWithTime("Adatbázisban található objektum sikeresen törölve."
                           , "Hiba történt a törlés végrehajtása közben!", toExecute);
		delete child;
	}
}

void MainWindow::on_actionMegtekint_s_triggered()
{
    QTreeWidget* tw = ui->trWLeft;
    if (tw->selectedItems().size() > 0)
    {
        QTreeWidgetItem* child = tw->selectedItems().at(0);
        if (child->parent())
        {
            QString parent = child->parent()->text(0);
            std::function<bool(QString&)> toExecute;
            QSqlQuery * q = new QSqlQuery(db);
            if (queries::TABLES == parent)
            {
                toExecute = [&](QString& failMessage) {
                    bool success = q->exec(QString("SELECT column_name, data_type \
                                                   , data_length, data_precision, nullable FROM user_tab_columns WHERE table_name = '%1'")
                            .arg(child->text(0)));
                    failMessage = q->lastError().text();
                    return success; };
            }
            else if (queries::INDEXES == parent)
            {
                toExecute = [&](QString& failMessage) {
                    bool success = q->exec(QString("SELECT index_type, table_owner, table_name \
                                                   , uniqueness FROM user_indexes WHERE index_name = '%1'")
                            .arg(child->text(0)));
                    failMessage = q->lastError().text();
                    return success; };
            }
            else if (queries::VIEWS == parent)
            {
                toExecute = [&](QString& failMessage) {
                    bool success = q->exec(QString("SELECT text, view_type, read_only FROM user_views WHERE view_name = '%1'")
                                           .arg(child->text(0)));
                    failMessage = db.lastError().text();
                    return success; };
            }
            else if (queries::FUNCTIONS == parent)
            {
                toExecute = [&](QString& failMessage)
                {
                    bool success = q->exec(QString("SELECT object_name, object_id, created \
                                                   FROM user_objects WHERE UPPER(OBJECT_TYPE) = 'FUNCTION' AND object_name = '%1'")
                                           .arg(child->text(0)));
                    failMessage = q->lastError().text();
                    return success;
                };
            }
            logger.logWithTime("Adatbázisban található objektum sikeresen lekérdezve."
                               , "Hiba történt a lekérdezés végrehajtása közben!", toExecute);
            if (q->isActive())
            {
                QSqlQueryModel* model = new QSqlQueryModel;
                model->setQuery(*q);
                QTableView *view = new QTableView(ui->tWUpper);
                view->setModel(model);
                const auto& colCount = model->columnCount();
                const auto width = ui->tWUpper->width() - 20;
                for (auto i = 0; i < colCount; ++i)
                {
                    view->setColumnWidth(i, width / colCount);
                }
                view->show();
                ui->tWUpper->addTab(view, child->text(0));
                ui->tWUpper->setCurrentWidget(view);
            }
        }
    }

}

MainWindow::~MainWindow()
{
	delete ui;
	delete progressBar;
	delete queries;
	delete &db;
	delete &logger;
}

void MainWindow::on_tWUpper_tabCloseRequested(int index)
{
	delete ui->tWUpper->widget(index);
}

void MainWindow::on_tWLower_tabCloseRequested(int index)
{
	delete ui->tWLower->widget(index);
}


void MainWindow::on_actionBet_lt_s_triggered()
{
    queries->load();
}

void MainWindow::on_actionMent_s_triggered()
{
    queries->save();
}
