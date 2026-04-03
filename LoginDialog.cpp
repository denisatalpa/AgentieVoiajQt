#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include "UserSession.h"

#include <QMessageBox>
#include <QDebug>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


LoginDialog::LoginDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::LoginDialogClass)
{
    ui->setupUi(this);
// apeleaza codul generat de qt designer care creeaza toti widgetii (qlabel, qlineedit etc)
// le seteaza pozitia dimensiunea
// apeleaza qmetaobject::connectslotsbyname() - conecteaza automat sloturile cu conventia on_numeobiect_eveniment
}

LoginDialog::~LoginDialog()
{
    delete ui;
// widgeturile vizuale sunt distruse automat de qt prin sistemul de parinti
// dar obiectul ui in sine trb sters manual pt ca l-am creat cu new si qt nu stie de el
}


void LoginDialog::on_loginButton_clicked()
{
    QString username = ui->usernameEdit->text().trimmed();
// trimmed elimina spatiile de la inceput si sfarsit din username
    QString password = ui->passwordEdit->text();
//la parola nu punem pt ca spatiile pot fi intentionate

    qDebug() << "Username: " << username;
    qDebug() << "Password: " << password;

    if (username.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "login esuat", "completati toate campurile");
        return;
    }

    /*
    // test sa vedem daca merge
    if (username == "test" && password == "1234")
    {
        QMessageBox::information(this, "login reusit", "bine ai venit!");
    }
    else
    {
        QMessageBox::critical(this, "login esuat", "username sau parola incorecta");
    }
    */

    QSqlQuery query;
    query.prepare(
        "SELECT id_user, tip_user FROM Utilizatori "
        "WHERE username = :username AND parola = :parola"
    );
    query.bindValue(":username", username);
    query.bindValue(":parola", password);
    query.exec();
// prepare cu placeholdere previne sql injection (daca userul scrie un cod sql in campul de username
// atunci bindvalue() il trateaza ca sir de caractere literal, nu ca cod sql

    if (query.next())
    {
        // salvam datele in sesiune
        UserSession::getInstance().login(
            query.value("id_user").toInt(),
            username,
            query.value("tip_user").toString()
        );

        //msj de bun venit
        QString tipUser = query.value("tip_user").toString();
        QMessageBox::information(this, "Succes",
            "Bine ai venit, " + username + "!\nTip cont: " + tipUser);
       
        accept(); // inchidem dialogul
    }
    else
    {
        QMessageBox::critical(this, "Eroare", "Username sau parola incorecta");
    }
}

