#ifndef DBLOGGER_HPP
#define DBLOGGER_HPP

#include <QObject>
#include <QPlainTextEdit>
#include <QProgressBar>

#include <QTime>
#include <QElapsedTimer>

#include <functional>

class DBLogger : public QPlainTextEdit
{
	Q_OBJECT

public:
	DBLogger(QWidget* parent = nullptr, QProgressBar* progressBar = nullptr)
		: QPlainTextEdit(parent), progressBar(progressBar)
	{
		QPlainTextEdit::setReadOnly(true);
	}

	void appendPlainText(const QString& text)
	{
		QPlainTextEdit::appendPlainText(QString("[%1] %2").arg(QTime::currentTime().toString()).arg(text));
	}
    bool logWithTime(const QString& success, const QString& fail, std::function<bool(QString&)>& slowFunc);
    void log(const QString text, const QString errorMessage = "");
	void setProgressBar(QProgressBar* pB)
	{
		progressBar = pB;
	}

private:
	QProgressBar* progressBar;
};

#endif
