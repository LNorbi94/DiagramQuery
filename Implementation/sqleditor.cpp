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
