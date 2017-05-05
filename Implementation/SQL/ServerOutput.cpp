#include "ServerOutput.h"

ServerOutput::ServerOutput(QSqlDatabase* db, QPlainTextEdit* dbmsOutput)
    : db(db)
    , dbmsOutput(dbmsOutput)
{
    QTime now = QTime::currentTime();
    qsrand(now.hour() + now.msec() + now.minute());
    const int rId = qrand() % 42;
    tableName = QString("tempForDbmsOutput%1").arg(rId);
    db->exec("BEGIN \
              DBMS_OUTPUT.ENABLE (buffer_size => NULL); \
            END;");
}

ServerOutput::~ServerOutput()
{
    db->exec(QString("DROP TABLE %1").arg(tableName));
    delete dbmsOutput;
}

void ServerOutput::writeOutput()
{
    db->exec(QString("DECLARE \
            v_status  INTEGER := 0; \
            v_line    VARCHAR2(100); \
            v_buff    VARCHAR2(1000); \
          BEGIN \
            WHILE v_status = 0 LOOP \
              DBMS_OUTPUT.GET_LINE (v_line, v_status); \
             IF v_status = 0 then \
             INSERT INTO %1 VALUES (v_line); \
            END IF; \
            END LOOP; \
          COMMIT; \
          END;").arg(tableName));
    QSqlQuery q;
    q = db->exec(QString("SELECT * FROM %1").arg(tableName));

    while (q.next())
    {
        const QString output = q.value(0).toString();
        dbmsOutput->appendPlainText(output);
    }

    clean();
}
