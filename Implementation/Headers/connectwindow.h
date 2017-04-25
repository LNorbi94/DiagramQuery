#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include "Headers/constants.h"
#include "Headers/mainwindow.h"

#include <QMainWindow>
#include <QShortcut>

#include <QXmlStreamWriter>
#include <QDir>

#include <QSqlDatabase>
#include <QSqlError>

#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui
{
    class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget* parent = nullptr);
    ~ConnectWindow();

    void fillConnectionList() noexcept;

    QString buildConnectionName(const QString& fname) const noexcept;

private slots:
    void on_pbConnect_clicked();
    void on_lwConnections_itemDoubleClicked(QListWidgetItem* item);
    void on_pbSave_clicked();
    void deleteConnection() noexcept;

private:
    Ui::ConnectWindow* ui;
};

#endif // CONNECTWINDOW_H
