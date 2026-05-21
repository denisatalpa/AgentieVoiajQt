#include "MainWindow.h"
#include "UserSession.h"
#include "ReservationDialog.h"
#include "ServerConnection.h" 

#include <QDebug>
#include <QMessageBox>

#include <QStringList> // pt lista de headeruri ale tabelului
#include <QTableWidgetItem> // pt celulele din tabel

#include <algorithm>   // pentru std::sort — demonstreaza operator


void MainWindow::incarcaOferte()
{
// curatam tabelul inante de a reincarca. daca nu am face ast, la fiecare
//..apel s-ar adauga randuri noi peste cele vechi, rezultand duplicate
    ui.tableOffers->setRowCount(0);
    m_oferte.clear();


    // Cerem ofertele de la server
    QStringList linii = ServerConnection::getInstance()
        .trimiteComanda("GET_OFFERS");

    // Fiecare linie: id|plecare|destinatie|dataPlecare|dataIntoarcere|
    //                transport|pretRegular|pretPremium|pretStandard|
    //                locuriRegular|locuriPremium
    int row = 0;
    for (const QString& linie : linii) {
        QStringList p = linie.split('|');
        if (p.size() < 11) continue;

        OfertaInfo o;
        o.id = p[0].toInt();
        o.plecare = p[1];
        o.destinatie = p[2];
        o.dataPlecare = p[3];
        o.dataIntoarcere = p[4];
        o.transport = p[5];
        o.pretRegular = p[6].toDouble();
        o.pretPremium = p[7].toDouble();
        o.locuriRegular = p[9].toInt();
        o.locuriPremium = p[10].toInt();

        qDebug() << o;   // operator
        m_oferte.append(o);

        auto item = [](const QString& t) {
            return new QTableWidgetItem(t);
            };

        ui.tableOffers->insertRow(row);
        ui.tableOffers->setItem(row, 0, item(o.plecare));
        ui.tableOffers->setItem(row, 1, item(o.destinatie));
        ui.tableOffers->setItem(row, 2, item(o.dataPlecare));
        ui.tableOffers->setItem(row, 3, item(o.dataIntoarcere));
        ui.tableOffers->setItem(row, 4, item(o.transport));
        ui.tableOffers->setItem(row, 5,
            item(QString::number(o.pretRegular, 'f', 2)));
        ui.tableOffers->setItem(row, 6,
            item(QString::number(o.pretPremium, 'f', 2)));
        ui.tableOffers->setItem(row, 7,
            item(QString::number(o.locuriRegular)));
        ui.tableOffers->setItem(row, 8,
            item(QString::number(o.locuriPremium)));
        row++;
    }

    // Demonstratie operator< : sortam o copie dupa pret
    QVector<OfertaInfo> sortate = m_oferte;
    std::sort(sortate.begin(), sortate.end());
    if (!sortate.isEmpty())
        qDebug() << "Cea mai ieftina oferta:" << sortate.first();


}


void MainWindow::actualizeazaStatus()
{
    if (UserSession::getInstance().esteLogat)
    {
        if (UserSession::getInstance().tipUser == "admin")
        {
            labelStatus->setText("Admin: " + UserSession::getInstance().username);
            labelStatus->setStyleSheet("color: red; font-weight: bold;");
        }
        else
        {
            labelStatus->setText("Logat ca: " + UserSession::getInstance().username);
            labelStatus->setStyleSheet("color: green; font-weight: bold;");
        }
        butonProfilulMeu->setVisible(true);
        ui.loginButton->setText("Delogare");
    }
// setstylesheet() aplica css inline unui widget. folosim culori diferite pt admin (rosu) si client (verde)
// ui.loginButton->setText() schimba textul butonului intre "autentificare" si "delogare"
//.. acelasi buton face 2 lucruri in functie de stare (toggle behaviour)

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
// de ce centralwidget si nu this (mainwindow) ? pt ca qmainwindow are o structura speciala:
//.. are menuBar, toolbar, status bar si in centru centralWidget
// widgeturile pe care le afisam merg in centralWidget


    labelStatus->setGeometry(10, 10, 200, 24);
    labelStatus->setStyleSheet("color: gray;");


    // cream butonul "contul meu" langa label
    butonProfilulMeu = new QPushButton("Contul meu", ui.centralWidget);
    butonProfilulMeu->setGeometry(10, 38, 100, 24);
    butonProfilulMeu->setVisible(false); // e ascuns pana cand ne logam
    connect(butonProfilulMeu, &QPushButton::clicked, this, &MainWindow::deschideProfilulMeu);
// butonul e creat invizibil (setvisible(false). il afisam doar dpa login in actualizeazastatus()
// dc? e mai elegant decat sa-l stergem si recreem


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

/*______________________________________________________________________________
// 
// conectarea la bd inainte sa o fi facut singleton
// 
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
// ___________________________________________________________________________ */


// Conectare la server (in loc de conectare directa la BD)
    if (!ServerConnection::getInstance().conecteaza("127.0.0.1", 12345)) {
        QMessageBox::critical(this, "Eroare",
            "Nu ma pot conecta la server!\nAsigura-te ca server.exe ruleaza.");
        return;
    }

    incarcaOferte();
}



MainWindow::~MainWindow()
{}



void MainWindow::on_loginButton_clicked()
{
    if (UserSession::getInstance().esteLogat)
    {
        //daca e logat, il delogam
        UserSession::getInstance().logout();
        actualizeazaStatus();
        return;
    }

    // factory method: nu stim (si nu ne intereseaza) cum e construit dialogul
    if (m_loginFactory.deschideDialog(this) == QDialog::Accepted)
        actualizeazaStatus();


/* nu ne mai trb partea ast pt ca am facut factory

	LoginDialog login(this);
    //cream cu this ca parinte pt ca mainwindow e printele dialogului

    if (login.exec() == QDialog::Accepted) {
        actualizeazaStatus(); // updatam label-ul dupa login
    }
    // .exec() deschide dialogul in mod modal, adica blocheaza fereastra princpala pana 
    // cand dialogul e inchis. daca foloseam .show() ar fi aparut fara sa blocheze nimic

// acelasi buton gestioneaza 2 stari: daca e logat atunci facem delogare, daca nu atunci deschide logindialog
// verificam rezultatul lui exec, daca userul a dat accepted(adica login reusit) actualizam statusul

*/

}


void MainWindow::on_signupButton_clicked()
{
    m_signupFactory.deschideDialog(this);

/* nu mai facem asa pt ca am facut factory

	SignUpDialog signup(this);
	signup.exec();
*/
}


void MainWindow::deschideProfilulMeu()
{
    m_profileFactory.deschideDialog(this);

/*
    ProfileDialog profil(this);
    profil.exec();
*/
}



void MainWindow::onOfertaSelectata(int row, int col)
{
    if (!UserSession::getInstance().esteLogat)
    {
        QMessageBox::warning(this, "Atentie", "Trebuie sa fii logat pentru a face o rezervare");
        return;
    }


/* ---------------------------------------------------------------------------
* nu ne mai trb partea asta pt ca am facut ofertaInfo
* 
    // luam datele ofertei din randul selectat
    int     idOferta = ui.tableOffers->item(row, 0)->data(Qt::UserRole).toInt();
    QString plecare = ui.tableOffers->item(row, 0)->text();
// luam id ul din userrole(int,curat) si textul vizibil din text()
// text() = bucuresti, data(qt::userrole) = 3 (id oferta din bd)

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

______________________________________________________________________________________
*/

    if (row < 0 || row >= m_oferte.size())
        return;

    // lua oferta din lista, folosim operator== daca vrem sa verificam
    const OfertaInfo& oferta = m_oferte[row];

    // dem operator== (verificam daca oferta mai are locuri)
    // construim o oferta "goala" cu 0 locuri si comparam id-ul
    OfertaInfo verificare;
    verificare.id = oferta.id;

    if (oferta == verificare)
    {
        // daca sunt egale dupa id, dar in practica nu ajungem aici pt ca verificare.id
        // e setat la fel. doar demontram functionalitatea lui ==
    }

    if (oferta.locuriRegular <= 0 && oferta.locuriPremium <= 0)
    {
        QMessageBox::information(this, "Oferta indisponibila",
            "Nu mai sunt locuri disponibile pentru aceasta oferta.");
        return;
    }


    ReservationDialog rezervare(
        this,
        oferta.id,
        oferta.plecare,
        oferta.destinatie,
        oferta.dataPlecare,
        oferta.dataIntoarcere,
        oferta.transport,
        oferta.pretRegular,
        oferta.pretPremium,
        oferta.locuriRegular,
        oferta.locuriPremium
    );


    if (rezervare.exec() == QDialog::Accepted) {
        incarcaOferte();
    }
    // dupa o rezervare reusita (accepted) reincarcam ofertele ca sa se updateze locurile disponibile
    // daca nu am face asta, tabelul ar afisa locuri vechi pana la urmatoarea deschidere a aplicatiei
}

