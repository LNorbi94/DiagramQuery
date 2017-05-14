#ifndef SERVEROUTPUT_H
#define SERVEROUTPUT_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QTime>

#include <QPlainTextEdit>
#include <QTabWidget>

class ServerOutput
{
public:
    ServerOutput(QSqlDatabase* db, QPlainTextEdit* dbmsOutput);
    ~ServerOutput();

    bool writeOutput();
    void prepareWrite()
    {
        db->exec("BEGIN \
                  DBMS_OUTPUT.ENABLE (buffer_size => NULL); \
                END;");
        db->exec(QString("CREATE TABLE %1 (line varchar2(100))").arg(tableName));
    }
    void clean()
    {
        db->exec(QString("DELETE FROM %1").arg(tableName));
        db->exec(QString("DROP TABLE %1").arg(tableName));
    }
    QPlainTextEdit* getOutput()
    { return dbmsOutput; }

private:
    QSqlDatabase* db;
    QPlainTextEdit* dbmsOutput;
    QString tableName;
};

#endif // SERVEROUTPUT_H
