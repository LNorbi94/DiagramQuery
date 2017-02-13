#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QWidget>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(QSqlDatabase * db, QWidget *parent);
    ~MainWindow();

private slots:
    void on_actionKil_p_s_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase * db;
};

#endif // MAINWINDOW_H
