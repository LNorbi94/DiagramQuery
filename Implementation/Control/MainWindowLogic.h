#ifndef MAINWINDOWLOGIC_H
#define MAINWINDOWLOGIC_H

#include <QSqlQueryModel>
#include <QTableView>

#include "SQL/SQLEditor.h"
#include "SQL/SQLHighlighter.h"
#include "Tools/Constants.h"
#include "Tools/DBLogger.h"

#include <QTreeWidgetItem>

class MainWindowLogic
{
public:
    MainWindowLogic(DBLogger* logger, QSqlDatabase* db, SqlEditor* queries);
    ~MainWindowLogic()
    {
        delete logger;
        delete queries;
    }

    void deleteDbObject(const QString& toDelete, const QString& parent);
    QTableView* viewDbObject(const QString& parent, const QString& child
                             , QTabWidget* editor);
    void dbObjectClicked(std::function<bool (QString&)>& toExecute);
    bool createList(const QString& queryToExecute
                    , QList<QTreeWidgetItem*>& list);
    QTreeWidget* createExecutionPlan(const int width);
    void executeString(const QString& query, QTabWidget* editor, QTabWidget* boxes);

private:
    DBLogger* logger;
    QSqlDatabase* db;
    SqlEditor* queries;
};

#endif // MAINWINDOWLOGIC_H
