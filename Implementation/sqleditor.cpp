#include "Headers/sqleditor.h"

QString SqlEditor::extractQuery()
{
	QTextCursor cursor(textCursor());
	cursor.beginEditBlock();
	cursor.clearSelection();
	cursor.movePosition(QTextCursor::PreviousWord);
	while (!cursor.selectedText().contains(";") && cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor));
	if (!cursor.atStart())
	{
		cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
	}
	cursor.clearSelection();
	while (!cursor.selectedText().contains(";") && cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor));
	cursor.endEditBlock();

	QString query = cursor.selectedText();
	query = query.remove(QRegExp("--([^\\s]| )*"));
	query = query.remove(QRegExp(";")).simplified();
	setTextCursor(cursor);
    return query;
}

void SqlEditor::save()
{
    QString fName = QFileDialog::getSaveFileName(this, tr("Lap mentése"), "", tr("Sql fájl (*.sql)"));
    if (!fName.isEmpty())
    {
        QFile file(fName);
        file.open(QFile::ReadWrite | QFile::Text);
        QTextStream writeF(&file);
        writeF << toPlainText();
    }
}

void SqlEditor::load()
{
    QString fName = QFileDialog::getOpenFileName(this);
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
    QString chart_type = queryInWords->at(2);
    bool ret = false;
    if ("PIECHART" == chart_type)
    {
        QT_CHARTS_USE_NAMESPACE
        auto byIterator = std::find(queryInWords->begin(), queryInWords->end(), "BY");
        ++byIterator;
        QRegExp regexp("\\((.)*");
        regexp.indexIn(query);
        QString queryToExecute = regexp.capturedTexts().at(0);
        q->exec(queryToExecute);

        if (!q->isActive())
        {
            message = q->lastError().text();
        }
        else if (q->record().count() != 2)
        {
            message = "A diagram működéséhez 2 oszlop szükséges.";
        }
        else
        {
            QPieSeries* series = new QPieSeries();

            while (q->next())
            {
                series->append(q->value(0).toString(), q->value(1).toInt());
            }

            chart->addSeries(series);
            chart->setTitle("Diagram rendezve a(z) " + *byIterator + " oszlop alapján.");
            chart->layout()->setContentsMargins(0, 0, 0, 0);
            chart->setTheme(QChart::ChartThemeDark);
            chart->setAnimationOptions(QChart::AllAnimations);

            ret = true;
        }
    }
    return ret;
}
