#include "MainWindow.h"
#include "LoginDialog.h"
#include "SignUpDialog.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QDebug>

#include <QDate>
#include <QStringList>
#include <QTableWidgetItem>


void MainWindow::incarcaOferte()
{
	QSqlQuery query("SELECT o.id_oferta, o.oras_plecare, o.oras_destinatie, o.data_plecare, o.data_intoarcere, t.tip, o.pret_standard "
		"FROM Oferta o "
		"JOIN Transporturi t ON o.id_transport = t.id_transport");

	int row = 0;
	while (query.next()) {
		ui.tableOffers->insertRow(row);
		ui.tableOffers->setItem(row, 0, new QTableWidgetItem(query.value("oras_plecare").toString()));
		ui.tableOffers->setItem(row, 1, new QTableWidgetItem(query.value("oras_destinatie").toString()));
		ui.tableOffers->setItem(row, 2, new QTableWidgetItem(query.value("data_plecare").toDate().toString("yyyy-MM-dd")));
		ui.tableOffers->setItem(row, 3, new QTableWidgetItem(query.value("data_intoarcere").toDate().toString("yyyy-MM-dd")));
		ui.tableOffers->setItem(row, 4, new QTableWidgetItem(query.value("tip").toString()));
		ui.tableOffers->setItem(row, 5, new QTableWidgetItem(QString::number(query.value("pret_standard").toFloat())));
		row++;
	}
}


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// ca sa punem numele coloanelor pt oferte in mainwindow (ca nu gaseam "horizontalTableLabels"
	ui.tableOffers->setColumnCount(6);
	QStringList headers;
	headers << "Plecare" << "Destinatie" << "Data plecare" << "Data intoarcere" << "Transport" << "Pret";
	ui.tableOffers->setHorizontalHeaderLabels(headers);

	// aici ca sa ne conectam la baza noastra de date
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); 
	db.setDatabaseName("Driver={SQL Server};Server=DESKTOP-ello;Database=AgentieVoiajDB;Trusted_Connection=yes;");

	if (!db.open()) {
		qDebug() << "Eroare la conectarea bazei: " << db.lastError().text();
		return;
	}

	incarcaOferte();
}

MainWindow::~MainWindow()
{}

void MainWindow::on_loginButton_clicked()
{
	LoginDialog login(this);
	login.exec();
}

void MainWindow::on_signupButton_clicked()
{
	SignUpDialog signup(this);
	signup.exec();
}

