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
#include <QBarSeries>
#include <QBarSet>
#include <QPieSlice>
#include <QGraphicsLayout>

#include <iostream>

class SqlEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit SqlEditor(QWidget* parent = nullptr) : QTextEdit(parent)
    {
        connect(this, &SqlEditor::cursorPositionChanged
                , this, &SqlEditor::highlightCurrentLine);
    }

    QString extractQuery() noexcept;

    void save() const;
    void load(const QString& filename = "");

    bool makeChart(QString& message
                , QtCharts::QChart* chart
                , QStringList *queryInWords
                , QString query
                , QSqlQuery* q);

private slots:
    void highlightCurrentLine();

};

#endif // SQLEDITOR_H
