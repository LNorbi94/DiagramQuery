#ifndef DBLOGGER_HPP
#define DBLOGGER_HPP

#include <QObject>
#include <QPlainTextEdit>

#include <QTime>
#include <QElapsedTimer>

#include <functional>

class DBLogger : public QPlainTextEdit
{
	Q_OBJECT

public:
	DBLogger(QWidget * parent = nullptr);

	void appendPlainText(const QString& text);
	bool logWithTime(const QString & success, const QString& fail, std::function<bool()>& slowFunc);

private:
	
};

#endif