#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QProgressBar>
#include <QPlainTextEdit>
#include <QShortcut>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>

#include <QTableView>
#include <QSqlDatabase>

#include <QSplitter>
#include <QStatusBar>
#include <QMenuBar>

#include "Control/MainWindowLogic.h"
#include "SQL/ServerOutput.h"
#include "SQL/SQLEditor.h"
#include "SQL/SQLHighlighter.h"
#include "Tools/Constants.h"
#include "Tools/DBLogger.h"
#include "Tools/GUITools.h"
#include "View/ConnectWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent) = delete;
    MainWindow(QSqlDatabase& db, QWidget* parent = nullptr);
    ~MainWindow()
    {
        delete serverOutput;
        delete &db;
        delete progressBar;
        delete editor;
        delete boxes;
        delete dbObjects;
    }

private slots:
    void dbObjectsClicked(QTreeWidgetItem* item, int column);
    void deleteDbObject();
    void viewDbObject();
    void closeEditor(int index)
    {
        if (index > 0)
            delete editor->widget(index);
    }
    void closeBoxes(int index)
    {
        if (index > 1)
            delete boxes->widget(index);
    }
    void loadPage()
    { queries->load(); }
    void savePage()
    { queries->save(); }
    void newPage();
    void exit();
    void showExecutionPlan();
    void executeSelection();
    void executeString(const QString& query);
    void executeQuery()
    { executeString(queries->extractQuery()); }
    void reconnect();

    void newConnection();

private:
    void setupDbObjects(const QString dbName, QTreeWidget* dbObjects);
    void registerShortcuts();
    void connectSignals();
    void createMenuBar();

    bool fillList(QTreeWidgetItem* list, const QString& queryToExecute) noexcept;
    bool fillTableList(QTreeWidgetItem* table)
    { return fillList(table, queries::GET_TABLES); }
    bool fillIndexList(QTreeWidgetItem* index)
    { return fillList(index, queries::GET_INDEXES); }
    bool fillViewList(QTreeWidgetItem* view)
    { return fillList(view, queries::GET_VIEWS); }
    bool fillFunctionList(QTreeWidgetItem* function)
    { return fillList(function, queries::GET_FUNCTIONS); }

private:
    QSqlDatabase& db;
    QProgressBar* progressBar;
    SqlEditor* queries;

    QTabWidget* editor;
    QTabWidget* boxes;
    QTreeWidget* dbObjects;

    int upperTabIndex;
    int lowerTabIndex;

    MainWindowLogic* logic;
    ServerOutput* serverOutput;
};

#endif // MAINWINDOW_H
