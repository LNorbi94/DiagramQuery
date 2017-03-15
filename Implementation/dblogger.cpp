#include "Headers/dblogger.hpp"

bool DBLogger::logWithTime(const QString & success, const QString& fail, std::function<bool(QString&)>& slowFunc)
{
	QElapsedTimer timer;
	int elapsedTime = 0;
	timer.start();
	progressBar->setRange(0, 0);
	QString errorMessage;
	bool ret = false;
    if ((ret = slowFunc(errorMessage)))
	{
		elapsedTime = timer.elapsed();
		appendPlainText(success);
		appendPlainText(QString::fromUtf8("A művelet végrehajtva %1 ms alatt.").arg(elapsedTime));
	}
	else
	{
		appendPlainText(fail);
		appendPlainText(errorMessage);
		progressBar->setRange(0, 100);
	}
	progressBar->setRange(0, 100);
	return ret;
}
