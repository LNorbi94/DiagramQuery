#include "View/MainWindow.h"

MainWindow::MainWindow(QSqlDatabase& database, QWidget * parent) :
    QMainWindow(parent)
    , db(database)
    , editor(new QTabWidget)
    , boxes(new QTabWidget)
    , dbObjects(new QTreeWidget)
    , upperTabIndex(0)
    , lowerTabIndex(0)
{
    QDesktopWidget* desktop = QApplication::desktop();
    QSize size(1024, 768);
    QRect tempGeometry = desktop->availableGeometry();
    if (tempGeometry.width() < 1200)
    {
        const int width = tempGeometry.width() - 200;
        const int height = tempGeometry.height() - 100;
        size = QSize(width, height);
    }
    QRect geometry = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter
                                         , size
                                         , desktop->availableGeometry());
    QWidget* widget = new QWidget(this);
    QSplitter* mainSplitter = new QSplitter;
    QSplitter* sideSplitter = new QSplitter;
    SqlHighlighter * highLighter = new SqlHighlighter();
    QStatusBar* statusBar = new QStatusBar;
    QVBoxLayout* layout = new QVBoxLayout;
    queries = new SqlEditor(editor);
    QPlainTextEdit* dbmsOutput = new QPlainTextEdit;
    dbmsOutput->setReadOnly(true);
    serverOutput = new ServerOutput(&db, dbmsOutput);
    const QString dbName = db.hostName();
    progressBar = new QProgressBar(statusBar);
    DBLogger* logger = new DBLogger(boxes, progressBar);

    widget->setLayout(layout);
    setCentralWidget(widget);
    setStatusBar(statusBar);
    setWindowIcon(QIcon(":/images/favicon"));
    setGeometry(geometry);

    layout->addWidget(mainSplitter);
    mainSplitter->addWidget(dbObjects);
    mainSplitter->addWidget(sideSplitter);
    mainSplitter->setSizes({ 150, 600 });

    sideSplitter->setOrientation(Qt::Vertical);
    sideSplitter->addWidget(editor);
    sideSplitter->addWidget(boxes);
    sideSplitter->setSizes({ 400, 200 });

    editor->setTabsClosable(true);
    editor->addTab(queries, dbName);
    editor->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    boxes->setTabsClosable(true);
    boxes->addTab(logger, "Adatbázis log");
    boxes->tabBar()->tabButton(0, QTabBar::RightSide)->resize(0, 0);
    boxes->addTab(dbmsOutput, "DBMS Output");
    boxes->tabBar()->tabButton(1, QTabBar::RightSide)->resize(0, 0);

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setMaximumSize(150, 18);
    highLighter->setDocument(queries->document());
    statusBar->addPermanentWidget(progressBar, 0);

    setupDbObjects(dbName, dbObjects);
    registerShortcuts();
    connectSignals();
    createMenuBar();

    logic = new MainWindowLogic(logger, &db, queries);
    logic->setBoxes(boxes);

    if (db.isOpen())
        logger->appendPlainText("Sikeresen csatlakozva az adatbázishoz!");
}

void MainWindow::setupDbObjects(const QString dbName, QTreeWidget* dbObjects)
{
    dbObjects->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem * item;
    item = new QTreeWidgetItem(QStringList(queries::TABLES));
    items.append(item);
    item = new QTreeWidgetItem(QStringList(queries::INDEXES));
    items.append(item);
    item = new QTreeWidgetItem(QStringList(queries::VIEWS));
    items.append(item);
    item = new QTreeWidgetItem(QStringList(queries::FUNCTIONS));
    items.append(item);
    dbObjects->insertTopLevelItems(0, items);
    dbObjects->setHeaderLabel(dbName);
}

void MainWindow::registerShortcuts()
{
    QShortcut* shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Return), queries);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::executeQuery);

    shortcut = new QShortcut(QKeySequence(Qt::Key_F4), queries);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::showExecutionPlan);

    shortcut = new QShortcut(QKeySequence(Qt::Key_F3), queries);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::executeSelection);

    connect(editor, &QTabWidget::currentChanged
                     , [&] (int index) { upperTabIndex = index; });
    connect(boxes, &QTabWidget::currentChanged
                     , [&] (int index) { lowerTabIndex = index; });

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W), this);
    connect(shortcut, &QShortcut::activated, [&]
    { closeEditor(upperTabIndex); });

    shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_W), this);
    connect(shortcut, &QShortcut::activated, [&]
    { closeBoxes(lowerTabIndex); });

    shortcut = new QShortcut(QKeySequence::Delete, dbObjects);
    connect(shortcut, &QShortcut::activated, this, &MainWindow::deleteDbObject);
}

void MainWindow::connectSignals()
{
    connect(dbObjects, &QTreeWidget::itemDoubleClicked
            , this, &MainWindow::dbObjectsClicked);
    connect(editor, &QTabWidget::tabCloseRequested
            , this, &MainWindow::closeEditor);
    connect(boxes, &QTabWidget::tabCloseRequested
            , this, &MainWindow::closeBoxes);
}

void MainWindow::createMenuBar()
{
    QMenuBar* menuBar = new QMenuBar;
    menuBar->setGeometry(0, 0, 1024, 21);
    menuBar->setNativeMenuBar(true);
    QMenu* menu = new QMenu;
    menu->setTitle("Fájl");
    menuBar->addMenu(menu);
    menu->addAction("Új", this, &MainWindow::newPage, QKeySequence::New);
    menu->addAction("Megnyitás...", this
                    , &MainWindow::loadPage, QKeySequence::Open);
    menu->addAction("Mentés másként...", this
                    , &MainWindow::savePage, QKeySequence::Save);
    menu->addAction("Kilépés", this
                    , &MainWindow::exit, QKeySequence(Qt::CTRL + Qt::Key_Q));

    menu = new QMenu;
    menu->setTitle("Szerkesztés");
    menuBar->addMenu(menu);

    menu->addAction("Parancs futtatása", this, &MainWindow::executeQuery);
    menu->addAction("Kijelölés futtatása", this, &MainWindow::executeSelection);
    menu->addAction("Lekérdezési terv megjelenítése"
                    , this, &MainWindow::showExecutionPlan);

    menu = new QMenu;
    menu->setTitle("Adatbázis");
    menuBar->addMenu(menu);

    menu->addAction("Újrakapcsolódás...", this, &MainWindow::reconnect);
    menu->addAction("Új kapcsolat", this, &MainWindow::newConnection);

    menu = new QMenu;
    menu->setTitle("Adatbázis objektumok");
    menuBar->addMenu(menu);

    menu->addAction("Megtekintés", this, &MainWindow::viewDbObject);
    menu->addAction("Törlés", this, &MainWindow::deleteDbObject);
    menuBar->show();
    setMenuBar(menuBar);
}

bool MainWindow::fillList(QTreeWidgetItem* list
                          , const QString& queryToExecute) noexcept
{
    setUpdatesEnabled(false);
    QList<QTreeWidgetItem*> items;
    const bool created = logic->createList(queryToExecute, items);
    list->addChildren(items);
    setUpdatesEnabled(true);
    return created;
}

void MainWindow::newPage()
{
    const bool confirmDelete = confirm("Új lap megerősítése"
                                       , confirms::NEW_PAGE, this);
    if (confirmDelete)
        queries->setText("");
}

void MainWindow::dbObjectsClicked(QTreeWidgetItem* item, int /*column*/)
{
    qDeleteAll(item->takeChildren());
    bool earlyReturn = false;
    bool (MainWindow::*toFill) (QTreeWidgetItem*);

    if (queries::TABLES == item->text(0))
    {
        toFill = &MainWindow::fillTableList;
    }
    else if (queries::INDEXES == item->text(0))
    {
        toFill = &MainWindow::fillIndexList;
    }
    else if (queries::VIEWS == item->text(0))
    {
        toFill = &MainWindow::fillViewList;
    }
    else if (queries::FUNCTIONS == item->text(0))
    {
        toFill = &MainWindow::fillFunctionList;
    }
    else
    {
        earlyReturn = true;
        viewDbObject();
    }

    if (earlyReturn)
        return;

    std::function<bool(QString&)> toExecute = [&] (QString& /*failMessage*/)
    {
        return (this->*toFill)(item);
    };

    logic->dbObjectClicked(toExecute);
}

void MainWindow::showExecutionPlan()
{
    QTreeWidget* tree;
    tree = logic->createExecutionPlan(boxes->width());
    if (tree->columnCount() != 1)
    {
        boxes->addTab(tree, "Lekérdezési terv");
        boxes->setCurrentWidget(tree);
    }
}

void MainWindow::executeSelection()
{
    QString query = queries->textCursor().selectedText().simplified();
    if (!queries->isPlSql(query))
        query = query.remove(';');

    executeString(query);
}

void MainWindow::executeString(const QString& query)
{
    serverOutput->prepareWrite();
    logic->executeString(query, editor);
    serverOutput->writeOutput();
}

void MainWindow::deleteDbObject()
{
    if (dbObjects->selectedItems().size() > 0)
	{
        QTreeWidgetItem* child = dbObjects->selectedItems().at(0);
        if (child->childCount() != 0)
            return;
        const QString toDelete = child->text(0);
        bool confirmDelete = confirm( "Törlés megerősítése"
        , QString("Biztosan kívánja törölni a %1 nevű objektumot?").arg(toDelete)
                                            , this);
        if (confirmDelete)
        {
            const QString parent = child->parent()->text(0);
            logic->deleteDbObject(toDelete, parent);
            delete child;
        }
    }
}

void MainWindow::viewDbObject()
{
    if (dbObjects->selectedItems().size() == 0)
        return;

    const QTreeWidgetItem* child = dbObjects->selectedItems().at(0);

    if (!child->parent())
        return;

    const QString parent = child->parent()->text(0);
    logic->viewDbObject(parent, child->text(0), editor);
}

void MainWindow::exit()
{
    const bool confirmExit = confirm("Kilépés megerősítése"
                                     , "Biztos ki szeretne lépni?", this);

    if (confirmExit)
    {
        this->close();
        this->destroy();
    }
}

void MainWindow::reconnect()
{
    QDialog dialog(this);
    QFormLayout form(&dialog);

    form.addRow(new QLabel(
                    "Kérem adja meg újra a felhasználónevét és jelszavát!"
                ));

    QLineEdit* userName = new QLineEdit(&dialog);
    form.addRow("Felhasználónév: ", userName);

    QLineEdit* password = new QLineEdit(&dialog);
    password->setEchoMode(QLineEdit::Password);
    form.addRow("Jelszó: ", password);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    buttonBox.button(QDialogButtonBox::Ok)->setText("Kapcsolódás");
    buttonBox.button(QDialogButtonBox::Cancel)->setText("Mégse");
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
                , "Felhasználónév megadása"
                , "Kérem adjon meg felhasználónevet!");
            return;
        }
        if (password->text().isEmpty())
        {
            QMessageBox::warning(
                this
                , "Jelszó megadása"
                , "Kérem adja meg jelszavát!");
            return;
        }
        db.open(userName->text(), password->text());
        password->setText("");
    }
}

void MainWindow::newConnection()
{
    const bool confirmNew = confirm("Új kapcsolat megerősítése"
                                    , confirms::NEW_CONNECTION, this);
    if (confirmNew)
    {
        ConnectWindow* cw = new ConnectWindow();
        cw->show();
        this->close();
        this->destroy();
    }
}
