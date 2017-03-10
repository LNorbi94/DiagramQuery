#ifndef SQLHIGHLIGHTER_HPP
#define SQLHIGHLIGHTER_HPP

#include <QObject>
#include <QSyntaxHighlighter>

class SqlHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT

public:
	SqlHighlighter(QTextDocument * parent = nullptr);
	~SqlHighlighter();

protected:
	void highlightBlock(const QString &text) override;

private:
	struct Rule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<Rule> _rules;

	QRegExp _commentExpr;

	QTextCharFormat _keywordForm;
	QTextCharFormat _commentForm;
};

#endif // SQLHIGHLIGHTER_HPP