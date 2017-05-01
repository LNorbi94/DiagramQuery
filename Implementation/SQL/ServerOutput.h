#ifndef SERVEROUTPUT_H
#define SERVEROUTPUT_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QTime>

#include <QPlainTextEdit>

class ServerOutput
{
public:
    ServerOutput(QSqlDatabase* db, QPlainTextEdit* dbmsOutput);
    ~ServerOutput();

    void writeOutput();
    void prepareWrite()
    { db->exec(QString("CREATE TABLE %1 (line varchar2(100) )").arg(tableName)); }
    void clean()
    { db->exec(QString("DELETE FROM %1").arg(tableName)); }

private:
    QSqlDatabase* db;
    QPlainTextEdit* dbmsOutput;
    QString tableName;
};

#endif // SERVEROUTPUT_H
