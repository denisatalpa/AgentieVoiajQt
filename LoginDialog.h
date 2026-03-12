#pragma once

#include "ui_LoginDialog.h"
#include <QDialog>


namespace Ui {
    class LoginDialogClass;
}


class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget* parent = nullptr);
    ~LoginDialog();


private slots: //aici punem sloturile
    void on_loginButton_clicked();

private:
    Ui::LoginDialogClass* ui;
};

