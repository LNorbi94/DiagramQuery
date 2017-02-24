#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

namespace connections
{
	const QString CONFIGFOLDER = "Connections";
}

namespace queries
{
    const QString GETTABLES = "SELECT object_name FROM dba_objects \
        WHERE object_type = 'TABLE' AND object_name NOT LIKE '%$%'";
    const QString GETINDEXES = "SELECT object_name FROM user_objects \
        WHERE object_type = 'INDEX' AND object_name NOT LIKE '%$%'";
    const QString GETVIEWS = "SELECT object_name FROM user_objects \
        WHERE object_type = 'VIEW' AND object_name NOT LIKE '%$%'";
    const QString GETFUNCTIONS = "SELECT object_name FROM user_objects \
        WHERE object_type = 'FUNCTION' AND object_name NOT LIKE '%$%'";
}

#endif // !CONSTANTS_H
