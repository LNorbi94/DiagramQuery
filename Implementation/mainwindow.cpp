#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , db(nullptr)
{
    ui->setupUi(this);
    QList<int> sizes;
    sizes << 150 << 600;
    ui->mainSplitter->setSizes(sizes);
    sizes.clear();
    sizes << 400 << 200;
    ui->splitter->setSizes(sizes);
    ui->treeWidget->setHeaderLabel("aramis.inf.elte.hu");
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    QTreeWidgetItem * table = new QTreeWidgetItem(QStringList("Táblák"));
    items.append(table);
    items.append(new QTreeWidgetItem(QStringList("Indexek")));
    items.append(new QTreeWidgetItem(QStringList("Nézetek")));
    items.append(new QTreeWidgetItem(QStringList("Metódusok")));
    items.append(new QTreeWidgetItem(QStringList("Függvények")));
    ui->treeWidget->insertTopLevelItems(0, items);
    table->insertChildren(0, items);
    table->setFirstColumnSpanned(true);
}

MainWindow::MainWindow(QSqlDatabase * db, QWidget *parent = nullptr) :
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
    sizes << 100 << 600;
    ui->mainSplitter->setSizes(sizes);
    ui->treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i)
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    ui->treeWidget->insertTopLevelItems(0, items);
    ui->treeWidget->setHeaderLabel(db->hostName());
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    //delete ui->tabWidget->widget(index);
}
*/

void MainWindow::on_actionKil_p_s_triggered()
{
    this->close();
    this->destroy();
}
