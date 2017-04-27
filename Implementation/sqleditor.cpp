#include "Headers/sqleditor.h"

QString SqlEditor::extractQuery() noexcept
{
    QTextCursor cursor(textCursor());

    bool plSql = false;
    bool atEnd;

    bool exit = false;

    cursor.movePosition(QTextCursor::PreviousCharacter
                                  , QTextCursor::KeepAnchor, 2);

    QString text = cursor.selectedText();

    atEnd = plSql = text.contains('/');

    while(!exit && cursor.movePosition(QTextCursor::PreviousWord
                              , QTextCursor::KeepAnchor))
    {
        text = cursor.selectedText();
        exit = text.contains('/') || text.contains(tokens::EMPTY_LINE);
    }

    QString query;

    if (!plSql)
    {
        cursor.setPosition(textCursor().position());
        cursor.beginEditBlock();
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::PreviousWord);
        while (!cursor.selectedText().contains(";")
               && cursor.movePosition(QTextCursor::PreviousWord
                                      , QTextCursor::KeepAnchor));
        if (!cursor.atStart())
        {
            cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        }
        cursor.clearSelection();
        while (!cursor.selectedText().contains(";")
               && cursor.movePosition(QTextCursor::NextWord
                                      , QTextCursor::KeepAnchor));
        cursor.endEditBlock();
    }
    else
    {
        if (!atEnd)
            cursor.clearSelection();
        exit = false;

        const QTextCursor::MoveOperation where = atEnd
                ? QTextCursor::PreviousWord
                : QTextCursor::NextWord;
        while(!exit && cursor.movePosition(where, QTextCursor::KeepAnchor))
        {
            const QString selectedText = cursor.selectedText();
            exit = selectedText.contains('/');
            if (atEnd)
                exit = selectedText.contains(tokens::EMPTY_LINE);
        }
    }
    setTextCursor(cursor);
    query = cursor.selectedText();
    query = query.remove(QRegExp("--[^\u2029]*"));
    query = query.remove(QRegExp("/\\*([^/]|[^*]/)*\\*/"));
    query = query.remove('/');

    if (!plSql)
        query = query.remove(';');

    return query.simplified();
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
        groupedBy->remove(')');
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
    } else if ("BARCHART" == chart_type && executed)
    {
        q->previous();
        QBarSeries* series = new QBarSeries();

        while (q->next())
        {
            QBarSet* temp = new QBarSet(q->value(0).toString());
            *temp << q->value(1).toInt();
            series->append(temp);
            ++queryCount;
        }

        chart->addSeries(series);
    } else
    {
        executed = false;
    }

    executed = queryCount <= 10;
    tooMany = !executed;

    notEnough = queryCount == 0;
    executed = !notEnough;

    if (executed)
    {
        chart->setTitle(
                    "Diagram rendezve a(z) " + *groupedBy + " oszlop alapján.");
        chart->layout()->setContentsMargins(0, 0, 0, 0);
        chart->setTheme(QChart::ChartThemeDark);
        chart->setAnimationOptions(QChart::AllAnimations);
    }
    else
    {
        if (tooMany)
            message = "Kérem ezt a diagramtípust kevesebb adattal használja.";
        else if (notEnough)
            message = "Üres lekérdezés.";
        else
            message = "Nem támogatott diagram típus: " + chart_type;
    }
    return executed;
}

void SqlEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(255, 255, 212);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
