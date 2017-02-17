#include "Headers/connectwindow.h"
#include "ui_connectwindow.h"
#include "Headers/constants.h"
#include "Headers/mainwindow.h"

#include <fstream>
#include <QXmlStreamWriter>
#include <QDir>

#include <QSqlDatabase>
#include <QSqlError>

#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>

ConnectWindow::ConnectWindow(QWidget *parent) :
    QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    fillConnectionList();
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

/**
 **  Fills ListWidget with connections (with xml extension) from default configfolder.
 **  One more side effect: it clears the ListWidget before inserting.
 */
void ConnectWindow::fillConnectionList()
{
	ui->lwConnections->clear();
	QDir dir(connections::CONFIGFOLDER);
	if ( Q_LIKELY(dir.exists()) )
	{
		QStringList filters;
		filters << "*.xml";
		dir.setNameFilters(filters);

		QFileInfoList list = dir.entryInfoList();
        QString connectionName;
		for (int i = 0; i < list.size(); ++i)
		{
            connectionName = list.at(i).baseName();
            ui->lwConnections->addItem(connectionName);
		}
	}
}


void ConnectWindow::on_pbConnect_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName(ui->lEHost->text());
    db.setDatabaseName(ui->lEService->text());
    db.setPort(ui->lEPort->text().toInt());
    if ( Q_LIKELY(db.open(ui->lEUsername->text()
                        , ui->lEPassword->text())) )
    {
        MainWindow * wa = new MainWindow(nullptr, &db);
        wa->show();
		this->close();
        this->destroy();
    } else
	{
		QMessageBox::critical(
			this,
			tr("Hiba történt kapcsolódás közben!"),
			db.lastError().text());
    }
	db.close();
}

void ConnectWindow::on_lwConnections_itemClicked(QListWidgetItem *item)
{
	QString filename = connections::CONFIGFOLDER;
	filename.append("\\");
	filename.append(item->text());
	filename.append(".xml");
	QFile file(filename);
	if ( Q_LIKELY(file.open(QIODevice::ReadOnly)) )
	{
		QXmlStreamReader xmlReader;
		xmlReader.setDevice(&file);
		while (!xmlReader.atEnd())
		{
			if (xmlReader.isStartElement())
			{
				QStringRef tagname = xmlReader.name();
				if (0 == tagname.compare("port"))
				{
					xmlReader.readNext();
					QStringRef port = xmlReader.text();
					ui->lEPort->setText(port.toString());
				}
				else if (0 == tagname.compare("host"))
				{
					xmlReader.readNext();
					QStringRef host = xmlReader.text();
					ui->lEHost->setText(host.toString());
				}
				else if (0 == tagname.compare("username"))
				{
					xmlReader.readNext();
					QStringRef username = xmlReader.text();
					ui->lEUsername->setText(*username.string());
				}
				else if (0 == tagname.compare("service"))
				{
					xmlReader.readNext();
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
	input.setLabelText("Kérem írja be milyen néven kívánja menteni a kapcsolatot!");
	input.setBaseSize(QSize(200, 100));
    if (input.exec())
	{
        if (!QDir(connections::CONFIGFOLDER).exists())
		{
			QDir().mkdir(connections::CONFIGFOLDER);
		}
		QString filename = connections::CONFIGFOLDER;
		QString connectionName = input.textValue();
		if (connectionName.isEmpty())
		{
			QMessageBox::warning(
				this,
				tr("Név megadása"),
				tr("Kérem adjon meg egy nevet, amivel később majd el lehet érni a kapcsolatot!"));
		}
		else
		{
			filename.append(QDir::separator());
			filename.append(connectionName);
			filename.append(".xml");
			QFile file(filename);
			if (file.exists())
			{
				QMessageBox::warning(
					this,
					tr("Létező név"),
					tr("Már létezik kapcsolat ilyen névvel! Kérem adjon meg más nevet."));
			}
			else {
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
		}
	}
}

