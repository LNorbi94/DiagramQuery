#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SqlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit SqlHighlighter(QTextDocument* parent = nullptr);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct Rule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<Rule> _rules;

    QTextCharFormat _keywordForm;
    QTextCharFormat _functionForm;
    QTextCharFormat _commentForm;
    QTextCharFormat _literalForm;
};

#endif // SQLHIGHLIGHTER_H
