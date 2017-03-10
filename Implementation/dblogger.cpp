#include "dblogger.hpp"

DBLogger::DBLogger(QWidget * parent) : QPlainTextEdit(parent)
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
	if (slowFunc())
	{
		elapsedTime = timer.elapsed();
		appendPlainText(success);
		appendPlainText(QString::fromLatin1("A művelet végrehajtva %1 ms alatt.").arg(elapsedTime));
		return true;
	}
	else
	{
		appendPlainText(fail);
		return false;
	}
}

