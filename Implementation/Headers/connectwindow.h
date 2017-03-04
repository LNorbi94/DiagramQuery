#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QShortcut>

namespace Ui
{
	class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();

    void fillConnectionList();

private slots:
    void on_pbConnect_clicked();
    void on_lwConnections_itemDoubleClicked(QListWidgetItem *item);
    void on_pbSave_clicked();
    void deleteConnection();

private:
    Ui::ConnectWindow *ui;
};

#endif // CONNECTWINDOW_H
