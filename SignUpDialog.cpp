#include "SignUpDialog.h"
#include "ui_SignUpDialog.h"

#include <QMessageBox> // pt ferestrele popup de eroare/succes
#include <QDebug> // pt msj in consola (fol la debugging)

#include <QtSql/QSqlQuery> // pt a rula query uri sql
#include <QtSql/QSqlError> // pt a citi msj de eroare sql


SignUpDialog::SignUpDialog(QWidget* parent)
	:QDialog(parent), // apelam constructorul clasei parinte
	ui(new Ui::SignUpDialog) // cream obiectul ui in memorie
{
	ui->setupUi(this); // leaga UI ul = deseneaza toate elementele vizuale in fereastra
}

SignUpDialog::~SignUpDialog()
{
	delete ui; // elibereaza memoria ocupata de obiectul ui
	// fara asta am avea memory leak
}

void SignUpDialog::on_creeazaContButton_clicked()
{
	// accesam campul cu numele numeEdit din UI, luam textul scris in el
	// si eliminam spatiile de la inceput si sfarsit (daca userul a apasat din greseala space)
	QString nume = ui->numeEdit->text().trimmed();
	QString prenume = ui->prenumeEdit->text().trimmed();
	QString username = ui->usernameEdit->text().trimmed();
	QString email = ui->emailEdit->text().trimmed();
	QString telefon = ui->telefonEdit->text().trimmed();
	QString parola = ui->parolaEdit->text();
	// la parola nu am pus trimmed pt ca spatiile pot fi intentionate


	// verificam ca s-au completat toate campurile
	// isEmpty returneaza true daca sirul nu contine nimic
	if (nume.isEmpty() || prenume.isEmpty() || username.isEmpty() ||
		email.isEmpty() || telefon.isEmpty() || parola.isEmpty())
	{
		QMessageBox::warning(this, "Eroare", "Completati toate campurile");
		return;
	}

	// verificam daca username ul exista deja
	// :username e un placeholder, nu punem direct textul userului
	// bindValue() - inlocuim placeholderul cu valoarea reala
	// e modul sigur de a introduce date - previne sql injction 
	// (atacuri in care userul scrie cod sql in camp)
	QSqlQuery checkQuery;
	checkQuery.prepare("SELECT id_user FROM Utilizatori WHERE username = :username");
	checkQuery.bindValue(":username", username);
	checkQuery.exec();


	// next() - incearca sa mearga la primul rezultat returnat
	// daca returneaza true inseamna ca exista deja un user cu acel username
	if (checkQuery.next())
	{
		QMessageBox::warning(this, "Eroare", "Username-ul este deja folosit");
		return;
	}

	// inseram noul user in bd
	QSqlQuery insertQuery;
	insertQuery.prepare(
		"INSERT INTO Utilizatori (username, parola, nume, prenume, email, telefon) "
		"VALUES (:username, :parola, :nume, :prenume, :email, :telefon)"
	);
	insertQuery.bindValue(":username", username);
	insertQuery.bindValue(":parola", parola);
	insertQuery.bindValue(":nume", nume);
	insertQuery.bindValue(":prenume", prenume);
	insertQuery.bindValue(":email", email);
	insertQuery.bindValue(":telefon", telefon);

	if (insertQuery.exec())
	{
		QMessageBox::information(this, "Succes", "Contul a fost creat cu succes");
		accept(); // inchidem dialogul
	}
	else
	{
		QMessageBox::critical(this, "Eroare", "Eroare la creare cont: " + insertQuery.lastError().text());
	}
}
