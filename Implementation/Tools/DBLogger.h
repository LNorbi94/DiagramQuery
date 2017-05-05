#ifndef DBLOGGER_H
#define DBLOGGER_H

#include <QTextStream>
#include <QDir>

#include <QPlainTextEdit>
#include <QProgressBar>
#include <QTabWidget>
#include <QTime>
#include <QElapsedTimer>

#include <functional>

#include "Constants.h"

class DBLogger : public QPlainTextEdit
{
    Q_OBJECT

public:
    DBLogger(QWidget* parent = nullptr, QProgressBar* pB = nullptr)
        : QPlainTextEdit(parent), progressBar(pB)
    {
        QPlainTextEdit::setReadOnly(true);
        if (Q_UNLIKELY(!QDir(logger::LOG_FOLDER).exists()))
        {
            QDir().mkdir(logger::LOG_FOLDER);
        }
        QString fname = logger::LOG_FOLDER;
        fname.append(QDir::separator());
        fname.append(QDate::currentDate().toString("yyyy_MM_dd"));
        fname.append(".log");
        logFile.setFileName(fname);
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);
        logFileStream.setDevice(&logFile);
    }

    void setProgressBar(QProgressBar* pB) noexcept
    { progressBar = pB; }

    void appendPlainText(const QString& text) noexcept
    {
        const QString message = QString("[%1] %2")
                .arg(QTime::currentTime().toString())
                .arg(text);

        QPlainTextEdit::appendPlainText(message);
        logFileStream << message << "\r\n";
        logFileStream.flush();
    }

    bool logWithTime(const QString& success
                     , const QString& fail
                     , std::function<bool (QString&)>& slowFunc);
    void log(const QString& text, const QString& errorMessage = "");

private:
    QProgressBar* progressBar;
    QFile logFile;
    QTextStream logFileStream;
};

#endif // DBLOGGER_H
