#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include <QMessageBox>
#include <QDebug>

LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::LoginDialogClass)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}


void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    qDebug() << "Username: " << username;
    qDebug() << "Password: " << password;

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "login esuat", "completati toate campurile");
        return;
    }

    // test sa vedem daca merge
    if (username == "test" && password == "1234")
    {
        QMessageBox::information(this, "login reusit", "bine ai venit!");
    }
    else
    {
        QMessageBox::critical(this, "login esuat", "username sau parola incorecta");
    }
}

