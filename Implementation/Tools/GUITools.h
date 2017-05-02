#ifndef GUITOOLS_H
#define GUITOOLS_H

#include <QMessageBox>
#include <QPushButton>
#include <QString>

bool confirm(const QString& title, const QString& message
             , QWidget* parent = nullptr);

#endif // GUITOOLS_H
