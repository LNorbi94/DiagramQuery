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

#include "sqlhighlighter.hpp"
#include "constants.h"
#include "dblogger.hpp"
#include "sqleditor.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent) = delete;
	MainWindow(QSqlDatabase& db, QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void on_trWLeft_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_tWUpper_tabCloseRequested(int index);
	void on_tWLower_tabCloseRequested(int index);
	void executeQuery();
	void showExecutionPlan();

	void on_actionT_rl_s_triggered();
	void on_actionKil_p_s_triggered()
	{
		this->close();
		this->destroy();
	}

private:
	bool fillList(QTreeWidgetItem* list, const QString& queryToExecute);
	bool fillTableList(QTreeWidgetItem* table)
	{
		return fillList(table, queries::GET_TABLES);
	}
	bool fillIndexList(QTreeWidgetItem* index)
	{
		return fillList(index, queries::GET_INDEXES);
	}
	bool fillViewList(QTreeWidgetItem* view)
	{
		return fillList(view, queries::GET_VIEWS);
	}
	bool fillFunctionList(QTreeWidgetItem* function)
	{
		return fillList(function, queries::GET_FUNCTIONS);
	}

	Ui::MainWindow* ui;
	QSqlDatabase& db;
	QProgressBar* progressBar;
	DBLogger& logger;
	SqlEditor* queries;
};

#endif // MAINWINDOW_H
