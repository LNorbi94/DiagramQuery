#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QProgressBar>

#include <QTextEdit>
#include <QPlainTextEdit>
#include <QShortcut>

#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>

#include <QFileSystemModel>
#include <QElapsedTimer>
#include <QTime>

#include "sqlhighlighter.hpp"
#include "Headers/constants.h"
#include "dblogger.hpp"

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
	void executeQuery();

private:
    bool fillList(QTreeWidgetItem * list, const QString& queryToExecute, const QString& queryCount);
	bool fillTableList(QTreeWidgetItem * table);
    bool fillIndexList(QTreeWidgetItem * index);
    bool fillViewList(QTreeWidgetItem * view);
    bool fillFunctionList(QTreeWidgetItem * function);

    Ui::MainWindow* ui;
    QSqlDatabase& db;
    QProgressBar* progressBar;
	DBLogger& logger;
	QTextEdit* queries;
};

#endif // MAINWINDOW_H
