#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QProgressBar>

#include <QTextEdit>
#include <QPlainTextEdit>

#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>

#include <QFileSystemModel>
#include <QElapsedTimer>
#include <QTime>

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent) = delete;
    MainWindow(QSqlDatabase& db, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionKil_p_s_triggered();
    void on_trWLeft_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_tWUpper_tabCloseRequested(int index);
	void on_tWLower_tabCloseRequested(int index);

private:
    bool fillList(QTreeWidgetItem * item, QSqlQuery& query, int tableSize = 0);
	bool fillTableList(QTreeWidgetItem * table);
    bool fillIndexList(QTreeWidgetItem * index);
    bool fillViewList(QTreeWidgetItem * view);
    bool fillFunctionList(QTreeWidgetItem * function);

    Ui::MainWindow *ui;
    QSqlDatabase& db;
    QProgressBar * progressBar;
	QPlainTextEdit& logger;
};

#endif // MAINWINDOW_H
