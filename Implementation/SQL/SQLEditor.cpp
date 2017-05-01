#include "SQL/SQLEditor.h"

QString SqlEditor::extractQuery() noexcept
{
    QTextCursor cursor(textCursor());
    QString query;
    QString text;
    bool exit = false;
    bool atStart = false;

    while(!exit && cursor.movePosition(QTextCursor::Up
                              , QTextCursor::KeepAnchor))
    {
        text = cursor.block().text();
        exit = text.isEmpty();
    }

    atStart = !exit;
    if (atStart)
    {
        while(cursor.movePosition(QTextCursor::PreviousCharacter
                                      , QTextCursor::KeepAnchor));
    }

    cursor.clearSelection();

    if (!atStart)
        cursor.movePosition(QTextCursor::NextCharacter);

    exit = false;

    while(!exit && cursor.movePosition(QTextCursor::Down
                              , QTextCursor::KeepAnchor))
    {
        text = cursor.block().text();
        exit = text.isEmpty();
    }

    if (!exit)
    {
        while(cursor.movePosition(QTextCursor::NextCharacter
                                  , QTextCursor::KeepAnchor));
    }
    else
    {
        cursor.movePosition(QTextCursor::PreviousCharacter
                                          , QTextCursor::KeepAnchor);
    }

    setTextCursor(cursor);
    query = cursor.selectedText();
    query = query.remove(QRegExp("--[^\xe2\x80\xa9]*"));
    query = query.remove(QRegExp("/\\*([^/]|[^*]/)*\\*/"));
    query = query.remove('/');
    query = query.simplified();

    if (!isPlSql(query))
    {
        query = query.remove(';');
    }

    return query;
}

bool SqlEditor::isPlSql(const QString &query)
{
    bool itIsPlSql = false;

    QStringList keywords;
    keywords << "DECLARE" << "BEGIN" << "CREATE OR REPLACE PROCEDURE"
             << "CREATE OR REPLACE FUNCTION" << "CREATE OR REPLACE PACKAGE"
             << "CREATE OR REPLACE TRIGGER";

    for (const QString& keyword : keywords)
    {
        itIsPlSql = itIsPlSql || query.startsWith(keyword, Qt::CaseInsensitive);
    }

    return itIsPlSql;
}

void SqlEditor::save() const
{
    const QString fName = QFileDialog::getSaveFileName(nullptr
                                                    , tr("Lap mentése")
                                                    , ""
                                                    , tr("Sql fájl (*.sql)"));

    if (Q_LIKELY(!fName.isEmpty()))
    {
        QFile file(fName);
        file.open(QFile::WriteOnly | QFile::Text);
        QTextStream writeF(&file);
        writeF << toPlainText();
    }
}

void SqlEditor::load(const QString &filename)
{
    QString fName = filename;

    if (Q_LIKELY(fName.isEmpty()))
        fName = QFileDialog::getOpenFileName(this);

    QFile file(fName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream readF(&file);
    readF.setAutoDetectUnicode(true);
    setText(readF.readAll());
}

bool SqlEditor::makeChart(QString& message
                        , QtCharts::QChart* chart
                        , QStringList* queryInWords
                        , QString query
                        , QSqlQuery* q)
{
    QT_CHARTS_USE_NAMESPACE
    bool executed = false;
    bool tooMany = false;
    bool notEnough = false;
    QString chart_type;

    auto groupedBy = std::find(queryInWords->begin()
                               , queryInWords->end(), "BY");
    const QRegExp regexp("\\((.)*");
    regexp.indexIn(query);
    const QString queryToExecute = regexp.capturedTexts().at(0);
    q->exec(queryToExecute);
    int queryCount = 0;

    if (queryInWords->size() >= 1)
    {
        chart_type = queryInWords->at(2);
    }

    if (groupedBy == queryInWords->end())
    {
        message = "Nem GROUP BY lekérdezés.";
    }
    else if (!q->isActive())
    {
        message = q->lastError().text();
    }
    else if (q->record().count() != 2)
    {
        message = "A diagram működéséhez 2 oszlop szükséges.";
    }
    else if (q->next() && !q->value(1).toInt())
    {
        message = "Második oszlopnak számot kell tartalmaznia!";
    }
    else
    {
        ++groupedBy;
        executed = true;
    }

    if ("PIECHART" == chart_type && executed)
    {
        q->previous();
        QPieSeries* series = new QPieSeries();

        while (q->next())
        {
            series->append(QString("%1 (%2)")
                           .arg(q->value(0).toString())
                           .arg(q->value(1).toInt())
                           , q->value(1).toInt());
            ++queryCount;
        }

        chart->addSeries(series);
        chart->legend()->setAlignment(Qt::AlignLeft);
    } else if ("BARCHART" == chart_type && executed)
    {
        q->previous();
        QBarSeries* series = new QBarSeries();

        while (q->next())
        {
            QBarSet* temp = new QBarSet(QString("%1 (%2)")
                                        .arg(q->value(0).toString())
                                        .arg(q->value(1).toInt()));
            *temp << q->value(1).toInt();
            series->append(temp);
            ++queryCount;
        }

        chart->addSeries(series);
        chart->legend()->setAlignment(Qt::AlignBottom);
    } else
    {
        executed = false;
        if (message.isEmpty())
            message = "Nem támogatott diagram típus: " + chart_type;
    }

    executed = queryCount <= 10;
    tooMany = !executed;

    notEnough = queryCount == 0;
    executed = !notEnough && !tooMany;

    if (executed)
    {
        chart->setTitle("Diagram rendezve a(z) " + *groupedBy + " oszlop alapján.");
        chart->layout()->setContentsMargins(0, 0, 0, 0);
        chart->legend()->setVisible(true);
        chart->setTheme(QChart::ChartThemeDark);
        chart->setAnimationOptions(QChart::AllAnimations);
    }
    else if (message.isEmpty())
    {
        if (tooMany)
            message = "Kérem ezt a diagramtípust kevesebb adattal használja.";
        else if (notEnough)
            message = "Üres lekérdezés.";
    }
    return executed;
}

void SqlEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    const QColor lineColor = QColor(255, 255, 212);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    setExtraSelections(extraSelections);
}
