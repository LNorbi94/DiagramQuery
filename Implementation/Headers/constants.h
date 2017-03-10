#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace connections
{
	const QString CONFIGFOLDER = "Connections";
}

namespace queries
{
	const QString TABLES = QString::fromLatin1("Táblák");
    const QString GET_TABLES = "SELECT object_name FROM user_objects \
        WHERE object_type = 'TABLE' AND object_name NOT LIKE '%$%'";
    const QString GET_TABLES_COUNT = "SELECT COUNT(*) FROM user_objects \
        WHERE object_type = 'TABLE' AND object_name NOT LIKE '%$%'";

	const QString INDEXES = QString::fromLatin1("Indexek");
    const QString GET_INDEXES = "SELECT object_name FROM user_objects \
        WHERE object_type = 'INDEX' AND object_name NOT LIKE '%$%'";
	const QString GET_INDEXES_COUNT = "SELECT COUNT(*) FROM user_objects \
        WHERE object_type = 'INDEX' AND object_name NOT LIKE '%$%'";

	const QString VIEWS = QString::fromLatin1("Nézetek");
    const QString GET_VIEWS = "SELECT object_name FROM user_objects \
        WHERE object_type = 'VIEW' AND object_name NOT LIKE '%$%'";
	const QString GET_VIEWS_COUNT = "SELECT COUNT(*) FROM user_objects \
        WHERE object_type = 'VIEW' AND object_name NOT LIKE '%$%'";

	const QString FUNCTIONS = QString::fromLatin1("Függvények");
    const QString GET_FUNCTIONS = "SELECT object_name FROM user_objects \
        WHERE object_type = 'FUNCTION' AND object_name NOT LIKE '%$%'";
	const QString GET_FUNCTIONS_COUNT = "SELECT COUNT(*) FROM user_objects \
        WHERE object_type = 'FUNCTION' AND object_name NOT LIKE '%$%'";
}

#endif // !CONSTANTS_H
