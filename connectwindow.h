#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui
{
	class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = 0);
    ~ConnectWindow();

	void loadConnections();

private slots:
    void on_pbConnect_clicked();

    void on_lwConnections_itemClicked(QListWidgetItem *item);

	void on_pbSave_clicked();

private:
    Ui::ConnectWindow *ui;
    size_t i;
};

#endif // CONNECTWINDOW_H
