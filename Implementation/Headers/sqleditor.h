#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include <QObject>
#include <QTextEdit>

class SqlEditor : public QTextEdit
{
    Q_OBJECT
public:
    SqlEditor(QWidget * parent = nullptr);

    QString extractQuery();
private:
};

#endif // SQLEDITOR_H
