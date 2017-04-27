#include "Headers/connectwindow.h"
#include "ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget* parent) :
    QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    fillConnectionList();
    QShortcut * shortcut;
    shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->lwConnections);
    QObject::connect(shortcut, &QShortcut::activated
                     , this, &ConnectWindow::deleteConnection);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

QString ConnectWindow::buildConnectionName(const QString& fname) const noexcept
{
    QString filename = connections::CONFIGFOLDER;
    filename.append(QDir::separator());
    filename.append(fname);
    filename.append(".xml");
    return filename;
}

void ConnectWindow::deleteConnection() noexcept
{
    QListWidgetItem* itemToDelete = ui->lwConnections->currentItem();
    QString filename = buildConnectionName(itemToDelete->text());
    QFile file(filename);

    if (Q_LIKELY(file.remove()))
    {
        delete itemToDelete;
    }
}

void ConnectWindow::fillConnectionList() noexcept
{
    ui->lwConnections->clear();

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
            ui->lwConnections->addItem(connectionName);
        }
    }
}

/*
 * Possible improvements: reduce ifs.
 *
 */
void ConnectWindow::on_lwConnections_itemDoubleClicked(QListWidgetItem* item)
{
    const QString filename = buildConnectionName(item->text());
    QFile file(filename);

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
                if (0 == tagname.compare(tr("port")))
                {
                    QStringRef port = xmlReader.text();
                    ui->lEPort->setText(port.toString());
                }
                else if (0 == tagname.compare(tr("host")))
                {
                    QStringRef host = xmlReader.text();
                    ui->lEHost->setText(host.toString());
                }
                else if (0 == tagname.compare(tr("username")))
                {
                    QStringRef username = xmlReader.text();
                    ui->lEUsername->setText(username.toString());
                }
                else if (0 == tagname.compare(tr("service")))
                {
                    QStringRef service = xmlReader.text();
                    ui->lEService->setText(service.toString());
                }
            }
            xmlReader.readNext();
        }
    }
}

void ConnectWindow::on_pbSave_clicked()
{
    QInputDialog input;
    input.setInputMode(QInputDialog::TextInput);
    input.setWindowTitle("Kapcsolat mentése");
    input.setOkButtonText("Mentés");
    input.setCancelButtonText("Mégse");
    input.setLabelText(
                "Kérem írja be milyen néven kívánja menteni a kapcsolatot!");
    input.setBaseSize(QSize(200, 100));

    if (!input.exec())
        return;
    if (Q_UNLIKELY(!QDir(connections::CONFIGFOLDER).exists()))
    {
        const bool dirCreated = QDir().mkdir(connections::CONFIGFOLDER);
        if (!dirCreated)
        {
            QMessageBox::warning(
                this
                , tr("Könyvtár létrehozása sikertelen!")
                , errors::DIR_NOT_CREATED);
            return;
        }
    }
    QString filename = connections::CONFIGFOLDER;
    QString connectionName = input.textValue();
    if (Q_UNLIKELY(connectionName.isEmpty()))
    {
        QMessageBox::warning(
            this
            , tr("Név megadása kötelező")
            , errors::EMPTY_NAME);
        return;
    }
    filename.append(QDir::separator());
    filename.append(connectionName);
    filename.append(".xml");
    QFile file(filename);
    if (Q_UNLIKELY(file.exists()))
    {
        QMessageBox::warning(
            this
            , tr("Létező név")
            , errors::ALREADY_EXISTS);
        return;
    }
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("Connection");
    stream.writeAttribute("name", connectionName);
    stream.writeTextElement("host", ui->lEHost->text());
    stream.writeTextElement("port", ui->lEPort->text());
    stream.writeTextElement("service", ui->lEService->text());
    stream.writeTextElement("username", ui->lEUsername->text());
    stream.writeEndElement();
    stream.writeEndDocument();
    fillConnectionList();
}

void ConnectWindow::on_pbConnect_clicked()
{
    QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QOCI"));
    db->setHostName(ui->lEHost->text());
    db->setDatabaseName(ui->lEService->text());
    db->setPort(ui->lEPort->text().toInt());

    if (Q_LIKELY(db->open(ui->lEUsername->text()
        , ui->lEPassword->text())))
    {
        ui->lEPassword->setText("");

        MainWindow * wa = new MainWindow(*db);
        wa->show();

        this->close();
        this->destroy();
    }
    else
    {
        QMessageBox::critical(
            this
            , tr("Hiba történt kapcsolódás közben!")
            , db->lastError().text());
    }
}
