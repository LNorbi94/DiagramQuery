#include "sqlhighlighter.hpp"

/*
 * Keywords from: https://docs.oracle.com/database/121/SQLRF/ap_keywd001.htm#SQLRF55621 .
 * They are for oracle sql.
 * 
 */
SqlHighlighter::SqlHighlighter(QTextDocument * parent)
	: QSyntaxHighlighter(parent)
{
	Rule rule;

	_keywordForm.setForeground(QColor(58, 115, 220));
	_keywordForm.setFontWeight(QFont::Bold);
	QStringList keywordPatterns;
	keywordPatterns << "\\baccess\\b"		<< "\\badd\\b"		<< "\\ball\\b"
					<< "\\balter\\b"		<< "\\band\\b"		<< "\\bany\\b"
					<< "\\bas\\b"			<< "\\basc\\b"		<< "\\baudit\\b"
					<< "\\bbetween\\b"		<< "\\bby\\b"		<< "\\bchar\\b"
					<< "\\bcheck\\b"		<< "\\bcluster\\b"	<< "\\bcolumn\\b"
					<< "\\bcolumn_value\\b"	<< "\\bcomment\\b"	<< "\\bcompress\\b"
					<< "\\bconnect\\b"		<< "\\bcreate\\b"	<< "\\bcurrent\\b"
					<< "\\bdate\\b"			<< "\\bdecimal\\b"	<< "\\bdefault\\b"
					<< "\\bdelete\\b"		<< "\\bdesc\\b"		<< "\\bdistinct\\b"
					<< "\\bdrop\\b"			<< "\\belse\\b"		<< "\\bexclusive\\b"
					<< "\\bexists\\b"		<< "\\bfile\\b"		<< "\\bfloat\\b"
					<< "\\bfor\\b"			<< "\\bfrom\\b"		<< "\\bgrant\\b"
					<< "\\bgroup\\b"		<< "\\bhaving\\b"	<< "\\bidentified\\b"
					<< "\\bimmediate\\b"	<< "\\bin\\b"		<< "\\bincrement\\b"
					<< "\\bindex\\b"		<< "\\binitial\\b"	<< "\\binsert\\b"
					<< "\\binteger\\b"		<< "\\bintersect\\b"<< "\\binto\\b"
					<< "\\bis\\b"			<< "\\blevel\\b"	<< "\\blike\\b"
					<< "\\block\\b"			<< "\\blong\\b"		<< "\\bmaxextents\\b"
					<< "\\bminus\\b"		<< "\\bmlslabel\\b"	<< "\\bmode\\b"
					<< "\\bnoaudit\\b"		<< "\\bmodify\\b"	<< "\\bnested_table_id\\b"
					<< "\\bnocompress\\b"	<< "\\bnot\\b"		<< "\\bnowait\\b"
					<< "\\bnull\\b"			<< "\\bnumber\\b"	<< "\\bof\\b"
					<< "\\boffline\\b"		<< "\\bon\\b"		<< "\\bonline\\b"
					<< "\\boption\\b"		<< "\\bor\\b"		<< "\\border\\b"
					<< "\\bpctfree\\b"		<< "\\bprior\\b"	<< "\\bpublic\\b"
					<< "\\braw\\b"			<< "\\brename\\b"	<< "\\bresource\\b"
					<< "\\brevoke\\b"		<< "\\brow\\b"		<< "\\bROWID\\b"
					<< "\\brownum\\b"		<< "\\brows\\b"		<< "\\bselect\\b"
					<< "\\bsession\\b"		<< "\\bset\\b"		<< "\\bshare\\b"
					<< "\\bsize\\b"			<< "\\bsmallint\\b"	<< "\\bstart\\b"
					<< "\\bsuccessful\\b"	<< "\\bsynonym\\b"	<< "\\bsysdate\\b"
					<< "\\btable\\b"		<< "\\bthen\\b"		<< "\\bto\\b"
					<< "\\btrigger\\b"		<< "\\buid\\b"		<< "\\bunion\\b"
					<< "\\bunique\\b"		<< "\\bupdate\\b"	<< "\\buser\\b"
					<< "\\bvalidate\\b"		<< "\\bvalues\\b"	<< "\\bvarchar\\b"
					<< "\\bvarchar2\\b"		<< "\\bview\\b"		<< "\\bwhenever\\b"
					<< "\\bwhere\\b"		<< "\\bwith\\b";
	foreach(const QString &pattern, keywordPatterns) {
		rule.pattern = QRegExp(pattern);
		rule.format = _keywordForm;
		_rules.append(rule);
	}

	_commentForm.setForeground(Qt::red);
	rule.pattern = QRegExp("--[^\n]*");
	rule.format = _commentForm;
	_rules.append(rule);

	_commentExpr = QRegExp("/\\*");
}

SqlHighlighter::~SqlHighlighter()
{
	
}

void SqlHighlighter::highlightBlock(const QString & text)
{
	QString alteredText = text.compare("ROWID") == 0 ? text : text.toLower();
	foreach(const Rule &rule, _rules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(alteredText);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(alteredText, index + length);
		}
	}
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = _commentExpr.indexIn(text);
}
