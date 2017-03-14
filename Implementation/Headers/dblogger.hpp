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
    DBLogger(QWidget * parent = nullptr, QProgressBar* progressBar = nullptr);

	void appendPlainText(const QString& text);
	bool logWithTime(const QString & success, const QString& fail, std::function<bool()>& slowFunc);
    void setProgressBar(QProgressBar* progressBar);

private:
    QProgressBar* progressBar;
};

#endif
