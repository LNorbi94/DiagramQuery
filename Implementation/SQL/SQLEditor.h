#ifndef SQLEDITOR_H
#define SQLEDITOR_H

#include "Tools/Constants.h"

#include <QTextEdit>
#include <QTextStream>
#include <QFileDialog>
#include <QChart>
#include <QTextBlock>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QGraphicsLayout>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>

class SqlEditor : public QTextEdit
{
    Q_OBJECT

public:
    explicit SqlEditor(QWidget* parent = nullptr) : QTextEdit(parent)
    {
        connect(this, &SqlEditor::cursorPositionChanged
                , this, &SqlEditor::highlightCurrentLine);

        setAcceptRichText(false);
        QFont font("Segoe UI", 11);
        setFont(font);

        const int tabSize = 4;
        QFontMetrics metrics(font);
        setTabStopWidth(tabSize * metrics.width(' '));
    }

    QString extractQuery() noexcept;

    void save() const;
    void load(const QString& filename = "");

    bool makeChart(QString& message
                , QtCharts::QChart* chart
                , QStringList *queryInWords
                , QString query
                , QSqlQuery* q);

    bool isPlSql(const QString& query);

private slots:
    void highlightCurrentLine();
};

#endif // SQLEDITOR_H
