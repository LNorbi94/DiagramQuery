#include "View/ConnectWindow.h"

ConnectWindow::ConnectWindow(QWidget* parent) :
    QMainWindow(parent)
{
    QShortcut* shortcut;
    QGridLayout* layout = new QGridLayout;
    QWidget* widget = new QWidget(this);
    QDesktopWidget* desktop = QApplication::desktop();
    QRect geometry = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter
                                         , QSize(800, 170)
                                         , desktop->availableGeometry());

    setGeometry(geometry);
    setWindowTitle("Kapcsolódás az adatbázishoz");
    setWindowIcon(QIcon(":/images/favicon"));

    setupLabels(layout);
    setupLineEdites(layout);
    setupButtons(layout);

    connectionList = new QListWidget();
    layout->addWidget(connectionList, 1, 3, 6, 1);
    shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), connectionList);
    connect( shortcut, &QShortcut::activated
             , this, &ConnectWindow::deleteConnection);

    shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    connect( shortcut, &QShortcut::activated
            , this, &ConnectWindow::connectToDatabase);

    widget->setLayout(layout);
    setCentralWidget(widget);
    setFixedSize(size());
    fillConnectionList();

    connect( connectionList, &QListWidget::itemDoubleClicked
            , this, &ConnectWindow::load);
}

void ConnectWindow::setupLabels(QGridLayout* layout)
{
    QLabel* label = new QLabel();
    label->setText("Hoszt és Port:");
    layout->addWidget(label, 0, 0);

    label = new QLabel();
    label->setText(":");
    layout->addWidget(label, 1, 1);

    label = new QLabel();
    label->setText("Mentett kapcsolatok:");
    layout->addWidget(label, 0, 3);

    label = new QLabel();
    label->setText("SSID:");
    layout->addWidget(label, 2, 0);

    label = new QLabel();
    label->setText("Felhasználónév:");
    layout->addWidget(label, 4, 0);

    label = new QLabel();
    label->setText("Jelszó:");
    layout->addWidget(label, 4, 2);
}

void ConnectWindow::setupLineEdites(QGridLayout* layout)
{
    QLineEdit* lineEdit = new QLineEdit();
    layout->addWidget(lineEdit, 1, 0);
    textFields["host"] = lineEdit;

    lineEdit = new QLineEdit();
    layout->addWidget(lineEdit, 1, 2);
    textFields["port"] = lineEdit;

    lineEdit = new QLineEdit();
    layout->addWidget(lineEdit, 3, 0, 1, 3);
    textFields["service"] = lineEdit;

    lineEdit = new QLineEdit();
    layout->addWidget(lineEdit, 5, 0);
    textFields["username"] = lineEdit;

    lineEdit = new QLineEdit();
    lineEdit->setEchoMode(QLineEdit::EchoMode::Password);
    layout->addWidget(lineEdit, 5, 1, 1, 2);
    textFields["password"] = lineEdit;
}

void ConnectWindow::setupButtons(QGridLayout* layout)
{
    QPushButton* pushButton = new QPushButton();
    pushButton->setText("Mentés");
    layout->addWidget(pushButton, 6, 0);
    connect( pushButton, &QPushButton::clicked
            , this, &ConnectWindow::save);

    pushButton = new QPushButton();
    pushButton->setText("Kapcsolódás");
    pushButton->setDefault(true);
    layout->addWidget(pushButton, 6, 1, 1, 2);
    connect( pushButton, &QPushButton::clicked
            , this, &ConnectWindow::connectToDatabase);
}

void ConnectWindow::deleteConnection() noexcept
{
    const QString toDelete = connectionList->currentItem()->text();
    const QString confirmDel = "Biztosan törölni kívánja a(z) %1 nevű kapcsolatot?";
    bool confirmDelete = confirm( "Törlés megerősítése"
                                  , confirmDel.arg(toDelete)
                                  , this);

    bool success = false;
    if (confirmDelete)
        success = logic.deleteConnection(connectionList->currentItem());

    if (!success)
    {
        QMessageBox::warning(
            this
            , "Kapcsolat törlése"
            , "Kapcsolat törlése sikertelen!");
    }
}

void ConnectWindow::fillConnectionList() noexcept
{
    connectionList->clear();
    connectionList->addItems(logic.createList());
}

void ConnectWindow::save()
{
    QInputDialog input(this);
    input.setInputMode(QInputDialog::TextInput);
    input.setWindowTitle("Kapcsolat mentése");
    input.setOkButtonText("Mentés");
    input.setCancelButtonText("Mégse");
    input.setLabelText("Kérem írja be milyen néven kívánja menteni a kapcsolatot!");
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
                , "Könyvtár létrehozása sikertelen!"
                , errors::DIR_NOT_CREATED);
            return;
        }
    }

    const QString connectionName = input.textValue();
    if (Q_UNLIKELY(connectionName.isEmpty()))
    {
        QMessageBox::warning(
            this
            , "Név megadása kötelező"
            , errors::EMPTY_NAME);
        return;
    }

    const QString fileName = logic.buildConnectionName(connectionName);
    QFile file(fileName);
    if (Q_UNLIKELY(file.exists()))
    {
        QMessageBox::warning(
            this
            , "Létező név"
            , errors::ALREADY_EXISTS);
        bool overWrite = confirm("Megerősítés"
                                 , "Szeretné felülírni a létező kapcsolatot?");

        if (!overWrite)
            return;
    }

    file.open(QIODevice::WriteOnly);
    if (Q_UNLIKELY(!file.isOpen()))
    {
        QMessageBox::warning(
            this
            , "Fájl megnyitása"
            , "Fájl megnyitása sikertelen!");
        return;
    }

    logic.save(connectionName, textFields, file);
    fillConnectionList();
}

void ConnectWindow::connectToDatabase()
{
    QSqlDatabase* db = new QSqlDatabase(QSqlDatabase::addDatabase("QOCI"));
    db->setHostName(textFields["host"]->text());
    db->setDatabaseName(textFields["service"]->text());
    db->setPort(textFields["port"]->text().toInt());

    if (Q_LIKELY(db->open(textFields["username"]->text()
        , textFields["password"]->text())))
    {
        textFields["password"]->setText("");

        MainWindow * wa = new MainWindow(*db);
        wa->show();

        this->close();
        this->destroy();
    }
    else
    {
        QMessageBox::critical(
            this
            , "Hiba történt kapcsolódás közben!"
            , db->lastError().text());
    }
}
