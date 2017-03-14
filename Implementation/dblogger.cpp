#include "Headers/dblogger.hpp"

DBLogger::DBLogger(QWidget * parent, QProgressBar* progressBar) : QPlainTextEdit(parent), progressBar(progressBar)
{
	QPlainTextEdit::setReadOnly(true);
}

void DBLogger::appendPlainText(const QString & text)
{
	QPlainTextEdit::appendPlainText(QString("[%1] %2").arg(QTime::currentTime().toString()).arg(text));
}

bool DBLogger::logWithTime(const QString & success, const QString& fail, std::function<bool()>& slowFunc)
{
	QElapsedTimer timer;
	int elapsedTime = 0;
	timer.start();
    progressBar->setRange(0, 0);
	if (slowFunc())
	{
		elapsedTime = timer.elapsed();
		appendPlainText(success);
        appendPlainText(QString::fromUtf8("A művelet végrehajtva %1 ms alatt.").arg(elapsedTime));
        progressBar->setRange(0, 100);
		return true;
	}
	else
	{
		appendPlainText(fail);
        progressBar->setRange(0, 100);
		return false;
    }
}

void DBLogger::setProgressBar(QProgressBar* pb)
{
    progressBar = pb;
}
