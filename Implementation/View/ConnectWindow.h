#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include "Control/ConnectWindowLogic.h"
#include "Tools/Constants.h"
#include "Tools/GUITools.h"
#include "View/MainWindow.h"

#include <QSqlDatabase>
#include <QSqlError>

#include <QInputDialog>
#include <QMessageBox>
#include <QShortcut>

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QLayout>

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget* parent = nullptr);
    void fillConnectionList() noexcept;

private slots:
    void connectToDatabase();
    void load(QListWidgetItem* item) noexcept
    { logic.load(textFields, item->text()); }
    void save();
    void deleteConnection() noexcept;

private:
    void setupLabels(QGridLayout* layout);
    void setupLineEdites(QGridLayout* layout);
    void setupButtons(QGridLayout* layout);

    std::map<QString, QLineEdit*> textFields;
    QListWidget* connectionList;
    ConnectWindowLogic logic;
};

#endif // CONNECTWINDOW_H
