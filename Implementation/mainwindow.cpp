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
    upperTabIndex = 0;
    lowerTabIndex = 0;

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

    QTreeWidget * treeWidget = ui->trWLeft;
    treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem * tables    = new QTreeWidgetItem(
                QStringList(queries::TABLES));
    QTreeWidgetItem * indexes   = new QTreeWidgetItem(
                QStringList(queries::INDEXES));
    QTreeWidgetItem * views     = new QTreeWidgetItem(
                QStringList(queries::VIEWS));
    QTreeWidgetItem * functions = new QTreeWidgetItem(
                QStringList(queries::FUNCTIONS));
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


bool MainWindow::fillList(QTreeWidgetItem* list
                          , const QString& queryToExecute) noexcept
{
    setUpdatesEnabled(false);

    QList<QTreeWidgetItem*> items;
    QSqlQuery query;
    query.setForwardOnly(true);
    query.exec(queryToExecute);

    if (!query.isActive())
    {
        return false;
    }

    while (query.next())
    {
        const QString item = query.value(0).toString();
        items.append(new QTreeWidgetItem({item}));
    }
    list->addChildren(items);

    setUpdatesEnabled(true);

    return true;
}

void MainWindow::registerShortcuts()
{
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), queries);
    QObject::connect(shortcut, &QShortcut::activated
                     , this, &MainWindow::executeQuery);

    shortcut = new QShortcut(QKeySequence(Qt::Key_F4), queries);
    QObject::connect(shortcut, &QShortcut::activated
                     , this, &MainWindow::showExecutionPlan);

    shortcut = new QShortcut(QKeySequence(Qt::Key_F3), queries);
    QObject::connect(shortcut, &QShortcut::activated
                     , this, &MainWindow::executeSelection);

    QObject::connect(ui->tWUpper, &QTabWidget::currentChanged
                     , [&] (int index) { upperTabIndex = index; });
    QObject::connect(ui->tWLower, &QTabWidget::currentChanged
                     , [&] (int index) { lowerTabIndex = index; });

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W)
                             , this);
    QObject::connect(shortcut, &QShortcut::activated, [&]
    {
        if (upperTabIndex != 0)
            on_tWUpper_tabCloseRequested(upperTabIndex);
    });

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
    QObject::connect(shortcut, &QShortcut::activated, [&]
    {
        if (lowerTabIndex != 0)
            on_tWLower_tabCloseRequested(lowerTabIndex);
    });
}

void MainWindow::on_trWLeft_itemDoubleClicked(QTreeWidgetItem* item
                                              , int /*column*/)
{
    qDeleteAll(item->takeChildren());
    bool earlyReturn = false;
    bool ret = false;

    if (queries::TABLES == item->text(0))
    {
        ret = fillTableList(item);
    }
    else if (queries::INDEXES == item->text(0))
    {
        ret = fillIndexList(item);
    }
    else if (queries::VIEWS == item->text(0))
    {
        ret = fillViewList(item);
    }
    else if (queries::FUNCTIONS == item->text(0))
    {
        ret = fillFunctionList(item);
    }
    else
    {
        earlyReturn = true;
    }

    std::function<bool(QString&)> toExecute = [&] (QString& /*failMessage*/)
    {
        return ret;
    };

    if (earlyReturn)
    {
        on_actionMegtekint_s_triggered();
        return;
    }

    logger.logWithTime("Adatbázisban található objektumok sikeresen lekérdezve."
                       , "Hiba történt a lekérdezés végrehajtása közben!"
                       , toExecute);
}

void MainWindow::showExecutionPlan()
{
    std::function<bool(QString&)> toExecute = [&] (QString& failMessage)
    {
        QSqlQuery * q = new QSqlQuery(db);
        QString query = queries->extractQuery();
        QString explainPlan = QString(
                    "EXPLAIN PLAN SET STATEMENT_ID = 'temp1' FOR %1"
                    ).arg(query);
        bool success = q->exec(explainPlan);
        if (!success)
        {
            return success;
        }
        success = q->exec("SELECT cardinality, \
            lpad(' ',level-1)||operation||' '||options||' '||object_name, \
            depth, io_cost, cpu_cost, parent_id, id \
            FROM PLAN_TABLE \
            CONNECT BY prior id = parent_id \
            AND prior statement_id = statement_id \
            START WITH id = 0 \
            AND statement_id = 'temp1' \
            ORDER BY depth");

        if (success)
        {
            QVector<QTreeWidgetItem*> plans;

            while (q->next())
            {
                const QString row = q->value(0).toString();
                const QString plan = q->value(1).toString().trimmed();
                const QString ioCost = q->value(3).toString();
                const QString cpuCost = q->value(4).toString();
                const int parent_id = q->value(5).isNull() ?
                    -1 : q->value(5).toInt();
                QTreeWidgetItem* temp = new QTreeWidgetItem(
                    QStringList({ plan, row, ioCost, cpuCost })
                    );
                plans.append(temp);
                if (parent_id >= 0 && parent_id < plans.size())
                {
                    plans.at(parent_id)->addChild(temp);
                }
            }

            QTreeWidget* tree = new QTreeWidget();
            tree->setColumnCount(4);
            tree->setHeaderLabels({ "Lekérdezési terv", "Sorok"
                        , "IO költség", "CPU költség" });
            QList<QTreeWidgetItem*> temp = plans.toList();
            tree->addTopLevelItem(temp.front());

            const int colCount = tree->columnCount() - 1;
            const int width = ui->tWUpper->width() - 20 - 500;
            tree->setColumnWidth(0, 500);
            for (int i = 1; i < colCount; ++i)
            {
                tree->setColumnWidth(i, width / colCount);
            }

            ui->tWLower->addTab(tree, "Lekérdezési terv");
            ui->tWLower->setCurrentWidget(tree);
            success = q->exec(
                     "DELETE FROM PLAN_TABLE WHERE statement_id = 'temp1'");
        }
        failMessage = q->lastError().text();
        delete q;
        return success;
    };

    logger.logWithTime("Lekérdezési terv sikeresen elkészült!"
                       , "Lekérdezési terv létrehozása sikertelen."
                       , toExecute);
}

void MainWindow::executeSelection()
{
    const QString query = queries->textCursor().selectedText().simplified();
    db.exec("CREATE TABLE temp (line varchar2(100) )");
    db.exec("BEGIN \
              DBMS_OUTPUT.ENABLE (buffer_size => NULL); \
          END;");
    if (!query.isEmpty())
        executeString(query);

    db.exec("DECLARE \
            v_status  INTEGER := 0; \
            v_line    VARCHAR2(100); \
            v_buff    VARCHAR2(1000); \
          BEGIN \
            WHILE v_status = 0 LOOP \
              DBMS_OUTPUT.GET_LINE (v_line, v_status); \
             IF v_status = 0 then \
             INSERT INTO temp VALUES (v_line); \
            END IF; \
            END LOOP; \
          COMMIT; \
          END;");
    QSqlQuery* q = new QSqlQuery();
    q->exec("select * from temp");

    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery(*q);
    QTableView* view = new QTableView(ui->tWLower);
    view->setModel(model);
    view->show();
    ui->tWLower->addTab(view, "Dbms Output");
    ui->tWLower->setCurrentWidget(view);
}

void MainWindow::executeString(const QString& query)
{
    if (query.isEmpty())
    {
        logger.log("Hiba: üres lekérdezést próbált végrehajtani!");
        return;
    }

    QSqlQuery * q = new QSqlQuery(db);
    std::function<bool(QString&)> toExecute = [&] (QString& failMessage)
    {
            q->exec(query);
            failMessage = q->lastError().text();
            return q->isActive();
    };

	QStringList words = QStringList(query.split(' '));
	for (auto& i : words)
    {
        i = i.toUpper();
    }

    if (words.size() >= 1 && "SELECT" == words.at(0))
    {
        const bool success = logger.logWithTime(
                                            "Lekérdezés sikeresen végrehajtva!"
                                          , "Lekérdezés sikertelen."
                                          , toExecute);
        if (success)
        {
            QSqlQueryModel* model = new QSqlQueryModel;
            model->setQuery(*q);
            QTableView* view = new QTableView(ui->tWLower);
            view->setModel(model);
            view->show();
            ui->tWLower->addTab(view, "Lekérdezés eredménye");
            ui->tWLower->setCurrentWidget(view);
        }
    }
    else if (words.size() >= 2 && "MAKE" == words.at(0))
    {
        if ("CHART" == words.at(1))
        {
            QString message;
            QT_CHARTS_USE_NAMESPACE
            QChart* chart = new QChart();
            bool success;

            toExecute = [&] (QString& msg)
            {
                success = queries->makeChart(msg
                                   , chart
                                   , &words
                                   , query
                                   , q);
                message = msg;
                return success;
            };

            logger.logWithTime("Diagram sikeresen létrehozva!"
                               , "Diagram létrehozása közben hiba történt."
                               , toExecute);

            if (success)
            {
                QChartView* chartView = new QChartView(chart);
                chartView->setRenderHint(QPainter::Antialiasing);
                ui->tWUpper->addTab(chartView, "Diagram eredménye");
                ui->tWUpper->setCurrentWidget(chartView);
            }
        }
    }
    else
    {
        logger.logWithTime("Művelet sikeresen végrehajtva."
                           , "Művelet sikertelen.", toExecute);
    }

    delete q;
}

void MainWindow::on_actionT_rl_s_triggered()
{
	QTreeWidget* tw = ui->trWLeft;
	if (tw->selectedItems().size() > 0)
	{
		QTreeWidgetItem* child = tw->selectedItems().at(0);
		QString parent = child->parent()->text(0);
        std::function<bool(QString&)> toExecute = [&](QString& failMessage)
        {
            bool success = false;
            if (queries::TABLES == parent)
            {
                success = db.exec(QString("DROP TABLE %1")
                                  .arg(child->text(0))).isActive();
            }
            else if (queries::INDEXES == parent)
            {
                success = db.exec(QString("DROP INDEX %1")
                                  .arg(child->text(0))).isActive();
            }
            else if (queries::VIEWS == parent)
            {
                success = db.exec(QString("DROP VIEW %1")
                                  .arg(child->text(0))).isActive();
            }
            else if (queries::FUNCTIONS == parent)
            {
                success = db.exec(QString("DROP FUNCTION %1")
                                  .arg(child->text(0))).isActive();
            }
            failMessage = db.lastError().text();
            return success;
        };
        logger.logWithTime("Adatbázisban található objektum sikeresen törölve."
                           , "Hiba történt a törlés végrehajtása közben!"
                           , toExecute);
        delete child;
    }
}

void MainWindow::on_actionMegtekint_s_triggered()
{
    QTreeWidget* tw = ui->trWLeft;
    if (tw->selectedItems().size() == 0)
    {
        return;
    }

    QTreeWidgetItem* child = tw->selectedItems().at(0);
    if (!child->parent())
    {
        return;
    }

    QString parent = child->parent()->text(0);
    std::function<bool(QString&)> toExecute;
    QSqlQuery * q = new QSqlQuery(db);
    toExecute = [&](QString& failMessage)
    {
        bool success = false;
        if (queries::TABLES == parent)
        {
            success = q->exec( queries::SELECT_TABLES(child->text(0)) );
        }
        else if (queries::INDEXES == parent)
        {
            success = q->exec( queries::SELECT_INDEXES(child->text(0)) );
        }
        else if (queries::VIEWS == parent)
        {
            success = q->exec( queries::SELECT_VIEWS(child->text(0)) );
        }
        else if (queries::FUNCTIONS == parent)
        {
            success = q->exec( queries::SELECT_FUNCTIONS(child->text(0)) );
        }
        failMessage = q->lastError().text();
        return success;
    };
    logger.logWithTime("Adatbázisban található objektum sikeresen lekérdezve."
                       , "Hiba történt a lekérdezés végrehajtása közben!"
                       , toExecute);

    if (q->isActive())
    {
        QSqlQueryModel* model = new QSqlQueryModel;
        model->setQuery(*q);
        QTableView *view = new QTableView(ui->tWUpper);
        view->setModel(model);
        const int colCount = model->columnCount();
        const int width = ui->tWUpper->width() - tables::SIDEBAR_SIZE;
        for (int i = 0; i < colCount; ++i)
        {
            view->setColumnWidth(i, width / colCount);
        }
        view->show();
        ui->tWUpper->addTab(view, child->text(0));
        ui->tWUpper->setCurrentWidget(view);
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

void MainWindow::on_action_jrakapcsol_d_s_triggered()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel(
                    tr("Kérem adja meg újra a felhasználónevét és jelszavát!")
                ));

    QLineEdit* userName = new QLineEdit(&dialog);
    form.addRow(tr("Felhasználónév: "), userName);

    QLineEdit* password = new QLineEdit(&dialog);
    password->setEchoMode(QLineEdit::Password);
    form.addRow(tr("Jelszó: "), password);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    buttonBox.button(QDialogButtonBox::Ok)->setText(tr("Kapcsolódás"));
    buttonBox.button(QDialogButtonBox::Cancel)->setText(tr("Mégse"));
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted
                     , &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected
                     , &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
    {
        if (userName->text().isEmpty())
        {
            QMessageBox::warning(
                this
                , tr("Felhasználónév megadása")
                , "Kérem adjon meg felhasználónevet!");
            return;
        }
        if (password->text().isEmpty())
        {
            QMessageBox::warning(
                this
                , tr("Jelszó megadása")
                , "Kérem adja meg jelszavát!");
            return;
        }
        db.open(userName->text(), password->text());
        password->setText("");
    }
}

void MainWindow::on_action_j_kapcsolat_triggered()
{
    ConnectWindow* cw = new ConnectWindow();
    cw->show();
    this->close();
    this->destroy();
}
