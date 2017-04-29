#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include "Tools/Constants.h"
#include "Headers/mainwindow.h"
#include "Control/ConnectWindowsLogic.h"

#include <QMainWindow>
#include <QShortcut>

#include <QSqlDatabase>
#include <QSqlError>

#include <QInputDialog>
#include <QMessageBox>

#include <QDesktopWidget>
#include <QLayout>

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget* parent = nullptr);

    void fillConnectionList() noexcept;

private slots:
    void on_pbConnect_clicked();
    void load(QListWidgetItem* item) noexcept;
    void save();
    void deleteConnection() noexcept;

private:
    void setupLabels(QGridLayout *layout);
    void setupLineEdites(QGridLayout *layout);
    void setupButtons(QGridLayout *layout);

    std::map<QString, QLineEdit*> textFields;
    QListWidget* connectionList;
    ConnectWindowsLogic logic;
};

#endif // CONNECTWINDOW_H
