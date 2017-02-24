#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QProgressBar>

#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>

#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent) = delete;
    MainWindow(QWidget *parent, QSqlDatabase& db);
    ~MainWindow();

private slots:
    void on_actionKil_p_s_triggered();
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    bool fillList(QTreeWidgetItem * item, QSqlQuery& query);
    bool fillTableList(QTreeWidgetItem * table);
    bool fillIndexList(QTreeWidgetItem * index);
    bool fillViewList(QTreeWidgetItem * view);
    bool fillFunctionList(QTreeWidgetItem * function);

    Ui::MainWindow *ui;
    QSqlDatabase& db;
    QProgressBar * progressBar;
};

#endif // MAINWINDOW_H
