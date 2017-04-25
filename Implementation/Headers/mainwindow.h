#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressBar>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>

#include <QTableView>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QFileSystemModel>
#include <QTextStream>

#include <QChartView>
#include <QGraphicsLayout>

#include <QtConcurrent/QtConcurrent>
#include <thread>

#include "sqlhighlighter.hpp"
#include "constants.h"
#include "dblogger.hpp"
#include "sqleditor.h"

#include <iostream>

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

    void on_actionT_rl_s_triggered();
    void on_actionKil_p_s_triggered()
    {
        this->close();
        this->destroy();
    }

    void on_actionMegtekint_s_triggered();

    void on_actionBet_lt_s_triggered();

    void on_actionMent_s_triggered();
    void showExecutionPlan();
    void executeSelection();
    void executeString(const QString& query);

    /*
     * Finds and executes query "near" the cursor in the current document.
     * It selects every text until EOL, from previous ;.
     *
     */
    void executeQuery()
    {
        QString query = queries->extractQuery();
        executeString(query);
        QSqlQuery q;
        q.exec("SET SERVEROUTPUT ON");
        std::cerr << q.lastError().databaseText().toStdString();
        q.exec("DBMS_OUTPUT.GET_LINES()");
        if (q.isActive())
        {
            while (q.next())
            {
                logger.log(q.value(0).toString());
            }
        }
        std::cerr << q.lastError().databaseText().toStdString();
    }

    void on_action_jrakapcsol_d_s_triggered();

private:
    bool fillList(QTreeWidgetItem* list, const QString& queryToExecute) noexcept;
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

    void registerShortcuts();

    DBLogger& logger;
    Ui::MainWindow* ui;
    QSqlDatabase& db;
    QProgressBar* progressBar;
    SqlEditor* queries;
};

#endif // MAINWINDOW_H
