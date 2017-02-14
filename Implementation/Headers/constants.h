#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace connections
{
	const QString CONFIGFOLDER = "Connections";
}

namespace queries
{
	const QString GETTABLES = "SELECT object_name FROM user_objects \
		WHERE object_type = 'TABLE' AND object_name NOT LIKE '%$%' AND ROWNUM < 20";
	const QString GETINDEXES = "SELECT object_name FROM user_objects \
		WHERE object_type = 'INDEX' AND object_name NOT LIKE '%$%' AND ROWNUM < 20";
	const QString GETVIEWS = "SELECT object_name FROM user_objects \
		WHERE object_type = 'VIEW' AND object_name NOT LIKE '%$%' AND ROWNUM < 20";
	const QString GETFUNCTIONS = "SELECT object_name FROM user_objects \
		WHERE object_type = 'FUNCTION' AND object_name NOT LIKE '%$%' AND ROWNUM < 20";
}

#endif // !CONSTANTS_H
