#include "ConnectWindowLogic.h"

QString ConnectWindowLogic::buildConnectionName(const QString &fname) const noexcept
{
    QString filename = connections::CONFIGFOLDER;
    filename.append(QDir::separator());
    filename.append(fname);
    filename.append(".xml");
    return filename;
}

bool ConnectWindowLogic::deleteConnection(QListWidgetItem* item)
{
    const QString filename = buildConnectionName(item->text());
    QFile file(filename);

    if (Q_LIKELY(file.remove()))
    {
        delete item;
        return true;
    }

    return false;
}

const QStringList ConnectWindowLogic::createList() const
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

void ConnectWindowLogic::save(const QString& connectionName
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

void ConnectWindowLogic::load(std::map<QString, QLineEdit *>& textFields
                               , const QString& connectionName) const
{
    for (auto& textField : textFields)
    {
        textField.second->setText("");
    }

    const QString filename = buildConnectionName(connectionName);
    QFile file(filename);

    if (Q_LIKELY(file.open(QIODevice::ReadOnly)))
    {
        QStringRef tagname;
        QStringRef text;
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);
        while (!xmlReader.atEnd())
        {
            if (xmlReader.isStartElement())
            {
                tagname = xmlReader.name();
                xmlReader.readNext();
                text = xmlReader.text();
                if ("port" == tagname)
                {
                    QIntValidator validator(0, 10000);
                    int pos = 0;
                    QString plainText = text.toString();
                    QValidator::State state = validator.validate(plainText
                                                                 , pos);
                    if (state == QValidator::Acceptable)
                        textFields["port"]->setText(text.toString());
                }
                else if ("host" == tagname)
                {
                    textFields["host"]->setText(text.toString());
                }
                else if ("username" == tagname)
                {
                    textFields["username"]->setText(text.toString());
                }
                else if ("service" == tagname)
                {
                    textFields["service"]->setText(text.toString());
                }
            }
            xmlReader.readNext();
        }
    }
}
