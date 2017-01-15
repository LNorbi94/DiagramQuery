#include "connectwindow.h"
#include "ui_connectwindow.h"
#include "constants.h"

#include <iostream>
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
    , i(0)
{
    ui->setupUi(this);
	loadConnections();
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::loadConnections()
{
	ui->lwConnections->clear();
	QDir dir(connections::CONFIGFOLDER);
	if (dir.exists())
	{
		QStringList filters;
		filters << "*.xml";
		dir.setNameFilters(filters);

		QFileInfoList list = dir.entryInfoList();
		for (int i = 0; i < list.size(); ++i)
		{
			QString name = list.at(i).baseName();
			ui->lwConnections->addItem(name);
		}
	}
}


void ConnectWindow::on_pbConnect_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("eszakigrid97@aramis.inf.elte.hu");
    db.setPort(1521);
    //db.setDatabaseName("customdb");
    db.setUserName("A8UZ7T");
    db.setPassword("fTzg35!");
    bool ok = db.open();
    if (ok)
    {
        QMessageBox popup;
        popup.setText("OK");
        popup.exec();
    } else {
        QMessageBox popup;
        popup.setText(db.lastError().text());
        popup.exec();
    }
}

void ConnectWindow::on_lwConnections_itemClicked(QListWidgetItem *item)
{
	QString filename = connections::CONFIGFOLDER;
	filename.append("\\");
	filename.append(item->text());
	filename.append(".xml");
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly))
	{
		QXmlStreamReader xmlReader;
		xmlReader.setDevice(&file);
		while (!xmlReader.atEnd())
		{
			if (xmlReader.isStartElement())
			{
				QStringRef tagname = xmlReader.name();
				if (tagname.compare("port") == 0)
				{
					xmlReader.readNext();
					QString port = xmlReader.text().toString();
					ui->tEditPort->setPlainText(port);
				}
				else if (tagname.compare("host") == 0)
				{
					xmlReader.readNext();
					QString host = xmlReader.text().toString();
					ui->tEditHost->setPlainText(host);
				}
				else if (tagname.compare("username") == 0)
				{
					xmlReader.readNext();
					QString username = xmlReader.text().toString();
					ui->tEditUsername->setPlainText(username);
				}
				else if (tagname.compare("service") == 0)
				{
					xmlReader.readNext();
					QString service = xmlReader.text().toString();
					ui->tEditService->setPlainText(service);
				}
			}
			xmlReader.readNext();
		}
	}
}

void ConnectWindow::on_pbSave_clicked()
{
	QInputDialog input;
	input.setCancelButtonText("Mégse");
	input.setInputMode(QInputDialog::TextInput);
	input.setOkButtonText("Mentés");
	input.setWindowTitle("Kapcsolat mentése");
	input.setLabelText("Kérem írja be milyen néven kívánja menteni a kapcsolatot!");
	input.setBaseSize(QSize(200, 100));
	if ( input.exec() )
	{
		if ( !QDir(connections::CONFIGFOLDER).exists() )
		{
			QDir().mkdir(connections::CONFIGFOLDER);
		}
		QString filename = connections::CONFIGFOLDER;
		QString title = input.textValue();
		filename.append("\\");
		filename.append(title);
		filename.append(".xml");
		std::cout << filename.toStdString();
		QFile file(filename);
		file.open(QIODevice::WriteOnly);
		QXmlStreamWriter stream(&file);
		stream.setAutoFormatting(true);
		stream.writeStartDocument();
		stream.writeStartElement("Connection");
		stream.writeAttribute("name", title);
		stream.writeTextElement("host", ui->tEditHost->toPlainText());
		stream.writeTextElement("port", ui->tEditPort->toPlainText());
		stream.writeTextElement("service", ui->tEditService->toPlainText());
		stream.writeTextElement("username", ui->tEditUsername->toPlainText());
		stream.writeEndElement();
		stream.writeEndDocument();
		loadConnections();
	}
}

