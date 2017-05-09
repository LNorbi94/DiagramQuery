#ifndef CONNECTWINDOWSLOGIC_H
#define CONNECTWINDOWSLOGIC_H

#include "Tools/Constants.h"

#include <QString>
#include <QDir>
#include <QXmlStreamWriter>

#include <QListWidgetItem>
#include <QLineEdit>

class ConnectWindowLogic
{
public:
    QString buildConnectionName(const QString& fname) const noexcept;
    bool deleteConnection(QListWidgetItem* item);
    const QStringList createList() const;
    void save (const QString& connectionName
               , const std::map<QString, QLineEdit*>& textFields
               , QFile& file) noexcept;
    void load (std::map<QString, QLineEdit*>& textFields
               , const QString& connectionName) const;
};

#endif // CONNECTWINDOWSLOGIC_H
