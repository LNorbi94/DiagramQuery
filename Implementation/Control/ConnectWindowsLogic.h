#ifndef CONNECTWINDOWSLOGIC_H
#define CONNECTWINDOWSLOGIC_H

#include "Tools/Constants.h"

#include <QString>
#include <QListWidgetItem>
#include <QLineEdit>

#include <QDir>
#include <QXmlStreamWriter>

#include <QtDebug>

class ConnectWindowsLogic
{

public:
    const QString buildConnectionName(const QString& fname) const noexcept;
    void deleteConnection(QListWidgetItem* item) noexcept;
    const QStringList createList() const noexcept;
    void save (const QString& connectionName
               , const std::map<QString, QLineEdit*>& textFields
               , QFile& file) noexcept;
    void load (std::map<QString, QLineEdit*>& textFields
               , const QString& connectionName) const noexcept;

};

#endif // CONNECTWINDOWSLOGIC_H
