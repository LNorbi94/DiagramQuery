#include "SQLHighlighter.h"

/*
 * Keywords from:
 * https://docs.oracle.com/database/121/SQLRF/ap_keywd001.htm#SQLRF55621 .
 * They are for oracle sql.
 * Functions from:
 * http://docs.oracle.com/cd/B19306_01/server.102/b14200/functions001.htm .
 */
SqlHighlighter::SqlHighlighter(QTextDocument * parent)
	: QSyntaxHighlighter(parent)
{
	Rule rule;
	QStringList keywordPatterns;

	keywordForm.setForeground(QColor(0, 0, 0));
	keywordForm.setFontWeight(QFont::Bold);
	functionForm.setForeground(QColor(70, 87, 137));
	functionForm.setFontWeight(QFont::Bold);

	keywordPatterns.reserve(128);
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
                    << "\\bnoaudit\\b"		<< "\\bmodify\\b"
                    << "\\bnested_table_id\\b"
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
					<< "\\bwhere\\b"		<< "\\bwith\\b"     << "\\bjoin\\b"
                    << "\\bnatural join\\b" << "\\breplace\\b" << "\\bdeclare\\b"
                    << "\\bbegin\\b" << "\\bend\\b" << "\\bexception\\b"
                    << "\\bif\\b" << "\\bwhen\\b" << "\\bloop\\b"
                    << "\\bthen\\b" << "\\bis\\b" << "\\bprocedure\\b"
                    << "\\bfunction\\b"  << "\\bpackage\\b";
	rule.format = keywordForm;
	for (auto& pattern : keywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rules.append(rule);
	}

	keywordPatterns.clear();
	keywordPatterns << "\\babs\\b"
					<< "\\bacos\\b" << "\\basin\\b" << "\\batan\\b"
					<< "\\batan2\\b" << "\\bbitand\\b" << "\\bceil\\b"
					<< "\\bcos\\b" << "\\bcosh\\b" << "\\bexp\\b"
					<< "\\bfloor\\b" << "\\bln\\b" << "\\blog\\b"
					<< "\\bmod\\b" << "\\bnanvl\\b" << "\\bpower\\b"
					<< "\\bremainder\\b" << "\\bround\\b" << "\\bsign\\b"
					<< "\\bsin\\b" << "\\bsinh\\b" << "\\bsqrt\\b"
					<< "\\btan\\b" << "\\btanh\\b" << "\\btrunc\\b"
					<< "\\bwidth_bucket\\b" << "\\bchr\\b" << "\\bconcat\\b"
					<< "\\binitcap\\b" << "\\blower\\b" << "\\blpad\\b"
                    << "\\bltrim\\b" << "\\bnls_initcap\\b"
                    << "\\bnls_lower\\b"
                    << "\\bnlssort\\b" << "\\bnls_upper\\b"
                    << "\\bregexp_replace\\b"
                    << "\\bregexp_substr\\b" << "\\brpad\\b"
					<< "\\brtrim\\b" << "\\bsoundex\\b" << "\\bsubstr\\b"
					<< "\\btranslate\\b" << "\\btreat\\b" << "\\btrim\\b"
                    << "\\bupper\\b" << "\\bnls_charset_decl_len\\b"
                    << "\\bnls_charset_id\\b"
                    << "\\bnls_charset_name\\b" << "\\bascii\\b"
                    << "\\binstr\\b"
                    << "\\blength\\b" << "\\bregexp_instr\\b"
                    << "\\badd_months\\b"
                    << "\\bcurrent_date\\b" << "\\bcurrent_timestamp\\b"
                    << "\\bdbtimezone\\b"
					<< "\\bextract\\b" << "\\bfrom_tz\\b" << "\\blast_day\\b"
                    << "\\blocaltimestamp\\b" << "\\bmonths_between\\b"
                    << "\\bnew_time\\b"
                    << "\\bnext_day\\b" << "\\bnumtodsinterval\\b"
                    << "\\bnumtoyminterval\\b"
                    << "\\bround\\b" << "\\bsessiontimezone\\b"
                    << "\\bsys_extract_utc\\b"
					<< "\\bsysdate\\b" << "\\bsystimestamp\\b" << "\\bto_char\\b"
                    << "\\bto_timestamp\\b" << "\\bto_timestamp_tz\\b"
                    << "\\bto_dsinterval\\b"
					<< "\\bto_yminterval\\b" << "\\btrunc\\b" << "\\btz_offset\\b"
					<< "\\basciistr\\b" << "\\bbin_to_num\\b" << "\\bcast\\b"
					<< "\\bchartorowid\\b" << "\\bcompose\\b" << "\\bconvert\\b"
                    << "\\bdecompose\\b" << "\\bhextoraw\\b"
                    << "\\bnumtodsinterval\\b" << "\\brawtonhex\\b"
                    << "\\bnumtoyminterval\\b" << "\\brawtohex\\b"
                    << "\\browidtochar\\b" << "\\browidtonchar\\b"
                    << "\\bscn_to_timestamp\\b"
                    << "\\btimestamp_to_scn\\b" << "\\bto_binary_double\\b"
                    << "\\bto_binary_float\\b"
					<< "\\bto_char\\b" << "\\bto_clob\\b" << "\\bto_date\\b"
                    << "\\bto_dsinterval\\b" << "\\bto_lob\\b"
                    << "\\bto_multi_byte\\b"
					<< "\\bto_nchar\\b" << "\\bto_nclob\\b" << "\\bto_number\\b"
                    << "\\bto_dsinterval\\b" << "\\bto_single_byte\\b"
                    << "\\bto_timestamp\\b"
                    << "\\bto_timestamp_tz\\b" << "\\bto_yminterval\\b"
                    << "\\bto_yminterval\\b"
					<< "\\bunistr\\b" << "\\bbfilename\\b" << "\\bempty_blob\\b"
					<< "\\bempty_clob\\b" << "\\bcardinality\\b" << "\\bcollect\\b"
                    << "\\bpowermultiset\\b" << "\\bpowermultiset_by_cardinality\\b"
                    << "\\bset\\b"
                    << "\\bsys_connect_by_path\\b" << "\\bcluster_id\\b"
                    << "\\bcluster_probability\\b"
                    << "\\bcluster_set\\b" << "\\bfeature_id\\b"
                    << "\\bfeature_set\\b"
                    << "\\bfeature_value\\b" << "\\bprediction\\b"
                    << "\\bprediction_cost\\b"
                    << "\\bprediction_details\\b" << "\\bprediction_probability\\b"
                    << "\\bprediction_set\\b"
					<< "\\bappendchildxml\\b" << "\\bdeletexml\\b" << "\\bdepth\\b"
					<< "\\bextract\\b" << "\\bexistsnode\\b" << "\\bextractvalue\\b"
                    << "\\binsertchildxml\\b" << "\\binsertxmlbefore\\b"
                    << "\\bpath\\b"
                    << "\\bsys_dburigen\\b" << "\\bsys_xmlagg\\b"
                    << "\\bsys_xmlgen\\b"
					<< "\\bupdatexml\\b" << "\\bxmlagg\\b" << "\\bxmlcdata\\b"
					<< "\\bxmlcolattval\\b" << "\\bxmlcomment\\b" << "\\bxmlconcat\\b"
					<< "\\bxmlforest\\b" << "\\bxmlparse\\b" << "\\bxmlpi\\b"
					<< "\\bxmlquery\\b" << "\\bxmlroot\\b" << "\\bxmlsequence\\b"
					<< "\\bxmlserialize\\b" << "\\bxmltable\\b" << "\\bxmltransform\\b"
					<< "\\bdecode\\b" << "\\bdump\\b" << "\\bora_hash\\b"
					<< "\\bvsize\\b" << "\\bcoalesce\\b" << "\\blnnvl\\b"
					<< "\\bnullif\\b" << "\\bnvl\\b" << "\\bnvl2\\b"
					<< "\\bsys_context\\b" << "\\bsys_guid\\b" << "\\bsys_typeid\\b"
					<< "\\buid\\b" << "\\buser\\b" << "\\buserenv\\b"
					<< "\\bcollect\\b" << "\\bcorr\\b" << "\\bcount\\b"
					<< "\\bcovar_pop\\b" << "\\bcovar_samp\\b" << "\\bcume_dist\\b"
					<< "\\bdense_rank\\b" << "\\bfirst\\b" << "\\bgroup_id\\b"
					<< "\\bgrouping\\b" << "\\bgrouping_id\\b" << "\\blast\\b"
					<< "\\bmax\\b" << "\\bmedian\\b" << "\\bmin\\b"
                    << "\\bpercentile_cont\\b" << "\\bpercentile_disc\\b"
                    << "\\bpercent_rank\\b"
                    << "\\brank\\b" << "\\bstats_binomial_test\\b"
                    << "\\bstats_crosstab\\b"
                    << "\\bstats_f_test\\b" << "\\bstats_ks_test\\b"
                    << "\\bstats_mode\\b"
                    << "\\bstats_mw_test\\b" << "\\bstats_one_way_anova\\b"
                    << "\\bstats_wsr_test\\b"
					<< "\\bstddev\\b" << "\\bstddev_pop\\b" << "\\bstddev_samp\\b"
					<< "\\bsum\\b" << "\\bvar_pop\\b" << "\\bvar_samp\\b"
					<< "\\bvariance\\b" << "\\bavg\\b" << "\\bcorr\\b"
					<< "\\bcovar_pop\\b" << "\\bcovar_samp\\b" << "\\bcume_dist\\b"
					<< "\\bdense_rank\\b" << "\\bfirst\\b" << "\\bfirst_value\\b"
					<< "\\blag\\b" << "\\blast\\b" << "\\blast_value\\b"
					<< "\\blead\\b" << "\\bntile\\b" << "\\bpercent_rank\\b"
                    << "\\bpercentile_cont\\b" << "\\bpercentile_disc\\b"
                    << "\\brank\\b"
					<< "\\bratio_to_report\\b" << "\\brow_number\\b" << "\\bstddev\\b"
					<< "\\bstddev_pop\\b" << "\\bstddev_samp\\b" << "\\bvar_pop\\b"
					<< "\\bvar_samp\\b" << "\\bvariance\\b" << "\\bcv\\b"
                    << "\\biteration_number\\b" << "\\bpresentnnv\\b"
                    << "\\bmake\\b" << "\\bchart\\b" << "\\bpiechart\\b"
                    << "\\bbarchart\\b" << "\\bpresentv\\b";
	rule.format = functionForm;
    for (const auto& pattern : keywordPatterns)
	{
		rule.pattern = QRegExp(pattern);
		rules.append(rule);
	}

	rule.pattern = QRegExp("\\btranslate\\b \\w+ \\busing\\b");
    rules.append(rule);

	commentForm.setForeground(Qt::gray);
	rule.pattern = QRegExp("--[^\\n]*");
	rule.format = commentForm;
	rules.append(rule);

    rule.pattern = QRegExp("/\\*([^/]|[^*]/)*\\*/");
    rules.append(rule);

	literalForm.setForeground(QColor(221, 17, 68));
    rule.pattern = QRegExp("'[^']*'");
	rule.format = literalForm;
	rules.append(rule);

}

void SqlHighlighter::highlightBlock(const QString & text)
{
    const QString alteredText = text.toLower();

    QRegExp expression("\\d+");
    const QRegExp word("\\w+");
	int index = expression.indexIn(alteredText);
    if (index == 0 || (index >= 1 && word.indexIn(alteredText.at(index - 1)) < 0))
	{
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, literalForm);
			index = expression.indexIn(alteredText, index + length);
		}
	}

    for (const auto& rule : rules)
	{
        expression = rule.pattern;
		int index = expression.indexIn(alteredText);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(alteredText, index + length);
		}
	}

	setCurrentBlockState(0);
}
