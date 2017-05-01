#include "DBLogger.h"

bool DBLogger::logWithTime(const QString & success, const QString& fail
                           , std::function<bool (QString&)>& slowFunc)
{
    QElapsedTimer timer;
    qint64 elapsedTime = 0;
    progressBar->setValue(100);
    progressBar->setRange(0, 0);
    QString errorMessage;
    timer.start();

    bool ret = false;
    if ((ret = slowFunc(errorMessage)))
    {
        elapsedTime = timer.elapsed();
        appendPlainText(success);
        appendPlainText(QString::fromUtf8("A művelet végrehajtva %1 ms alatt.")
                        .arg(elapsedTime));
    }
    else
    {
        log(fail, errorMessage);
    }

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    return ret;
}

void DBLogger::log(const QString& text, const QString& errorMessage)
{
    appendPlainText(text);
    if (!errorMessage.isEmpty())
    {
        appendPlainText(errorMessage);
    }
}
