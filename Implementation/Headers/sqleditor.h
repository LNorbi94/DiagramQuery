#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include <QObject>
#include <QTextEdit>

class SqlEditor : public QTextEdit
{
	Q_OBJECT

public:
	SqlEditor(QWidget* parent = nullptr) : QTextEdit(parent) { }

	QString extractQuery();
private:

};

#endif // SQLEDITOR_H
