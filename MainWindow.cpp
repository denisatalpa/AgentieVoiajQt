#include "MainWindow.h"
#include "LoginDialog.h"
#include "SignUpDialog.h"
#include "UserSession.h"
#include "ReservationDialog.h"
#include "ProfileDialog.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord> // pt a accesa structura rezultatelor(coloane)
#include <QDebug>
#include <QMessageBox>

#include <QDate>
#include <QStringList> // pt lista de headeruri ale tabelului
#include <QTableWidgetItem> // pt celulele din tabel


void MainWindow::incarcaOferte()
{
    // curatam tabelul inante de a reincarca
    ui.tableOffers->setRowCount(0);

    // cand cream un obiect cu query ul in paranteza, acesta se executa automat
    // nu mai trb sa apelam .exec() separat
    QSqlQuery query(
        "SELECT o.oras_plecare, o.oras_destinatie, o.data_plecare, o.data_intoarcere, t.tip, "
        
        "o.pret_standard * cr.multiplicator AS pret_regular, "
        "o.pret_standard * cp.multiplicator AS pret_premium, "

        // asta e un subquery (query in query ul mare)
        // ia totalul locurilor din transporturi, scade suma locurilor deja rezervate pt oferta respectiva
        // ISNULL(...,0) - daca nu exista nicio rezervare, SUM returneaza NULL, iar noi il
        // inlocuim cu 0 ca sa nu stricam calculul
        "t.locuri_totale_regular - ISNULL(( "
        "    SELECT SUM(r.numar_locuri) FROM Rezervari r "
        "    JOIN Categorii_Bilete cb ON r.id_categorie = cb.id_categorie "
        "    WHERE r.id_oferta = o.id_oferta AND cb.tip_clasa = 'Regular' "
        "), 0) AS locuri_regular, "

        "t.locuri_totale_premium - ISNULL(( "
        "    SELECT SUM(r.numar_locuri) FROM Rezervari r "
        "    JOIN Categorii_Bilete cb ON r.id_categorie = cb.id_categorie "
        "    WHERE r.id_oferta = o.id_oferta AND cb.tip_clasa = 'Premium' "
        "), 0) AS locuri_premium "

        "FROM Oferta o "
        "JOIN Transporturi t ON o.id_transport = t.id_transport "

        // facem join de 2 ori pe aceeasi tabelta categorii_bilete dar cu aliasuri diferite
        // cr = regular, cp = premium, ca sa putem lua ambele multiplicatoare in acelasi timp
        "JOIN Categorii_Bilete cr ON cr.id_transport = t.id_transport AND cr.tip_clasa = 'Regular' "
        "JOIN Categorii_Bilete cp ON cp.id_transport = t.id_transport AND cp.tip_clasa = 'Premium'"
    );

    int row = 0;
    while (query.next()) { // merge la urmatorul rand din rezultat
        // returneaza false daca nu mai sunt randuri
        // row tine evidetna pe ce linie din tabel inseram

        ui.tableOffers->insertRow(row); // adaugam randul

        // salvam id_oferta ascuns in primul item (nu il afisam dar il folosim la rezervare)
        QTableWidgetItem* itemId = new QTableWidgetItem(query.value("id_oferta").toString());

        itemId->setData(Qt::UserRole, query.value("id_oferta").toInt());

        //setItem(row, coloana, item) - pune o celula noua la pozitia specificata
        //query.value("oras_plecare") - ia valoarea coloanei cu numele dat din rezultatul query ului
        // new qtablewidgetitem() cream celula vizuala cu textul respectiv
        ui.tableOffers->setItem(row, 0, new QTableWidgetItem(query.value("oras_plecare").toString()));
        ui.tableOffers->setItem(row, 1, new QTableWidgetItem(query.value("oras_destinatie").toString()));
        ui.tableOffers->setItem(row, 2, new QTableWidgetItem(query.value("data_plecare").toDate().toString("yyyy-MM-dd")));
        ui.tableOffers->setItem(row, 3, new QTableWidgetItem(query.value("data_intoarcere").toDate().toString("yyyy-MM-dd")));
        ui.tableOffers->setItem(row, 4, new QTableWidgetItem(query.value("tip").toString()));
        ui.tableOffers->setItem(row, 5, new QTableWidgetItem(QString::number(query.value("pret_regular").toDouble(), 'f', 2)));
        ui.tableOffers->setItem(row, 6, new QTableWidgetItem(QString::number(query.value("pret_premium").toDouble(), 'f', 2)));
        ui.tableOffers->setItem(row, 7, new QTableWidgetItem(query.value("locuri_regular").toString()));
        ui.tableOffers->setItem(row, 8, new QTableWidgetItem(query.value("locuri_premium").toString()));
        
        // salvam id_oferta si preturile in UserRole ca sa le accesam la click
        ui.tableOffers->item(row, 0)->setData(Qt::UserRole, query.value("id_oferta").toInt());
        ui.tableOffers->item(row, 5)->setData(Qt::UserRole, query.value("pret_regular").toDouble());
        ui.tableOffers->item(row, 6)->setData(Qt::UserRole, query.value("pret_premium").toDouble());
        ui.tableOffers->item(row, 7)->setData(Qt::UserRole, query.value("locuri_regular").toInt());
        ui.tableOffers->item(row, 8)->setData(Qt::UserRole, query.value("locuri_premium").toInt());
        
        row++;
    }
}


void MainWindow::actualizeazaStatus()
{
    if (UserSession::esteLogat)
    {
        if (UserSession::tipUser == "admin")
        {
            labelStatus->setText("Admin: " + UserSession::username);
            labelStatus->setStyleSheet("color: red; font-weight: bold;");
        }
        else
        {
            labelStatus->setText("Logat ca: " + UserSession::username);
            labelStatus->setStyleSheet("color: green; font-weight: bold;");
        }
        butonProfilulMeu->setVisible(true);
        ui.loginButton->setText("Delogare");
    }
    else
    {
        labelStatus->setText("Guest");
        labelStatus->setStyleSheet("color: gray;");
        butonProfilulMeu->setVisible(false);
        ui.loginButton->setText("Autentificare");
    }
}




MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    // cream labelul de status in stanga sus
    labelStatus = new QLabel("Guest", ui.centralWidget);
    labelStatus->setGeometry(10, 10, 200, 24);
    labelStatus->setStyleSheet("color: gray;");

    // cream butonul "contul meu" langa label
    butonProfilulMeu = new QPushButton("Contul meu", ui.centralWidget);
    butonProfilulMeu->setGeometry(10, 38, 100, 24);
    butonProfilulMeu->setVisible(false); // e ascuns pana cand ne logam
    connect(butonProfilulMeu, &QPushButton::clicked, this, &MainWindow::deschideProfilulMeu);


	// ca sa punem numele coloanelor pt oferte in mainwindow (ca nu gaseam "horizontalTableLabels"
    //qstringlist e practic un vector<QString>
    // << e operatorl de adaugare in lista
    ui.tableOffers->setColumnCount(9);
    QStringList headers;
    headers << "Plecare" << "Destinatie" << "Data plecare" << "Data intoarcere"
        << "Transport" << "Pret Regular" << "Pret Premium"
        << "Locuri Regular" << "Locuri Premium";
    ui.tableOffers->setHorizontalHeaderLabels(headers); //pune numele coloanelor in headerul tabelului


    //click pe o linie din tabel
    connect(ui.tableOffers, &QTableWidget::cellClicked, this, &MainWindow::onOfertaSelectata);


	// aici ca sa ne conectam la baza noastra de date

    //cream o conexiune la bd folosind driverul QODBC (e driverul qt care foloseste
    // odbc pt a vorbi cu sql server
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC"); 

    //aici punem ce driver sa fol, pe ce server, ce bd
    //Trusted_connection=yes foloseste autentificarea windows, nu username/parola sql
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
    if (UserSession::esteLogat)
    {
        //daca e logat, il delocam
        UserSession::logout();
        actualizeazaStatus();
        return;
    }


	LoginDialog login(this);
    //cream cu this ca parinte pt ca mainwindow e printele dialogului

    if (login.exec() == QDialog::Accepted) {
        actualizeazaStatus(); // updatam label-ul dupa login
    }
    // .exec() deschide dialogul in mod modal, adica blocheaza fereastra princpala pana 
    // cand dialogul e inchis. daca folosea .show() ar fi aparut fara sa blocheze nimic
}


void MainWindow::on_signupButton_clicked()
{
	SignUpDialog signup(this);
	signup.exec();
}


void MainWindow::deschideProfilulMeu()
{
    ProfileDialog profil(this);
    profil.exec();
}



void MainWindow::onOfertaSelectata(int row, int col)
{
    if (!UserSession::esteLogat)
    {
        QMessageBox::warning(this, "Atentie", "Trebuie sa fii logat pentru a face o rezervare");
        return;
    }

    // luam datele ofertei din randul selectat
    int     idOferta = ui.tableOffers->item(row, 0)->data(Qt::UserRole).toInt();
    QString plecare = ui.tableOffers->item(row, 0)->text();
    QString destinatie = ui.tableOffers->item(row, 1)->text();
    QString dataPlecare = ui.tableOffers->item(row, 2)->text();
    QString dataIntoarcere = ui.tableOffers->item(row, 3)->text();
    QString transport = ui.tableOffers->item(row, 4)->text();
    double  pretRegular = ui.tableOffers->item(row, 5)->data(Qt::UserRole).toDouble();
    double  pretPremium = ui.tableOffers->item(row, 6)->data(Qt::UserRole).toDouble();
    int     locuriRegular = ui.tableOffers->item(row, 7)->data(Qt::UserRole).toInt();
    int     locuriPremium = ui.tableOffers->item(row, 8)->data(Qt::UserRole).toInt();

    ReservationDialog rezervare
    (
        this,
        idOferta, plecare, destinatie,
        dataPlecare, dataIntoarcere, transport,
        pretRegular, pretPremium,
        locuriRegular, locuriPremium
    );

    if (rezervare.exec() == QDialog::Accepted) {
        incarcaOferte(); // reincarcam ofertele ca sa se updateze locurile
    }

}

