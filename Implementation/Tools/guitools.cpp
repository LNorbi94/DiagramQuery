#include "GUITools.h"

bool confirm(const QString& title, const QString& message
             , QWidget* parent)
{
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);

    QAbstractButton* yesButton = msgBox.addButton("Igen", QMessageBox::YesRole);
    msgBox.addButton("Nem", QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    return msgBox.clickedButton() == yesButton;
}
