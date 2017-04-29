#include "ConnectWindowsLogic.h"

const QString ConnectWindowsLogic::buildConnectionName(const QString &fname) const noexcept
{
    QString filename = connections::CONFIGFOLDER;
    filename.append(QDir::separator());
    filename.append(fname);
    filename.append(".xml");
    return filename;
}

void ConnectWindowsLogic::deleteConnection(QListWidgetItem* item) noexcept
{
    const QString filename = buildConnectionName(item->text());
    QFile file(filename);

    if (Q_LIKELY(file.remove()))
    {
        delete item;
    }
}

const QStringList ConnectWindowsLogic::createList() const noexcept
{
    QStringList connectionList;
    QDir dir(connections::CONFIGFOLDER);
    if (Q_LIKELY(dir.exists()))
    {
        QStringList filters;
        filters << "*.xml";
        dir.setNameFilters(filters);

        const QFileInfoList list = dir.entryInfoList();
        QString connectionName;
        for (int i = 0; i < list.size(); ++i)
        {
            connectionName = list.at(i).baseName();
            connectionList.append(connectionName);
        }
    }
    return connectionList;
}

void ConnectWindowsLogic::save(const QString &connectionName
                               , const std::map<QString, QLineEdit*>& textFields
                               , QFile& file) noexcept
{
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Connection");
    stream.writeAttribute("name", connectionName);
    stream.writeTextElement("host", textFields.at("host")->text());
    stream.writeTextElement("port", textFields.at("port")->text());
    stream.writeTextElement("service", textFields.at("service")->text());
    stream.writeTextElement("username", textFields.at("username")->text());
    stream.writeEndElement();
    stream.writeEndDocument();
}

void ConnectWindowsLogic::load(std::map<QString, QLineEdit *>& textFields
                               , const QString& connectionName) const noexcept
{
    QString filename = buildConnectionName(connectionName);
    QFile file(filename);
    using QObject;

    if (Q_LIKELY(file.open(QIODevice::ReadOnly)))
    {
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        while (!xmlReader.atEnd())
        {
            if (xmlReader.isStartElement())
            {
                QStringRef tagname = xmlReader.name();
                xmlReader.readNext();
                QStringRef text = xmlReader.text();
                if (0 == tagname.compare(tr("port")))
                {
                    textFields["port"]->setText(text.toString());
                }
                else if (0 == tagname.compare(tr("host")))
                {
                    textFields["host"]->setText(text.toString());
                }
                else if (0 == tagname.compare(tr("username")))
                {
                    textFields["username"]->setText(text.toString());
                }
                else if (0 == tagname.compare(tr("service")))
                {
                    textFields["service"]->setText(text.toString());
                }
            }
            xmlReader.readNext();
        }
    }
}
