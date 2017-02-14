#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>

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
    MainWindow(QWidget *parent = nullptr);
    MainWindow(QWidget *parent, QSqlDatabase * db);
    ~MainWindow();

private slots:
    void on_actionKil_p_s_triggered();

private:
	bool fillList(QTreeWidgetItem * item, QSqlQuery * query);
	bool fillTableList(QTreeWidgetItem * table);
	bool fillIndexList(QTreeWidgetItem * index);
	bool fillViewList(QTreeWidgetItem * view);
	bool fillFunctionList(QTreeWidgetItem * function);

    Ui::MainWindow *ui;
    QSqlDatabase * db;
};

#endif // MAINWINDOW_H
