#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include <QObject>
#include <QTextEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QChart>

#include <functional>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QGraphicsLayout>

class SqlEditor : public QTextEdit
{
	Q_OBJECT

public:
	SqlEditor(QWidget* parent = nullptr) : QTextEdit(parent) { }

	QString extractQuery();

    void save();
    void load();

    bool makeChart(QString& message
                                                        , QtCharts::QChart* chart
                                                        , QStringList *queryInWords
                                                        , QString query
                                                        , QSqlQuery* q);
private:

};

#endif // SQLEDITOR_H
