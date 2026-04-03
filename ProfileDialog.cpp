#include "ProfileDialog.h"
#include "UserSession.h"

#include <QVBoxLayout> // pt layout vertical
#include <QHBoxLayout> // pt layout orizontal
#include <QFormLayout> // pt layout de tip formular (labelul in stanga si campul in dreapta)
#include <QGroupBox> // un dreptunghi cu titlu, ca sa grupeze vizual widgeturile
#include <QMessageBox> // fereastra popup ok/cancel/yes/no
#include <QHeaderView> // headerul tabelului 
#include <QDate> // tip de data calendaristica in qt

#include <QtSql/QSqlQuery> // executarea queryurilor sql
#include <QtSql/QSqlError> // info despre erori sql



ProfileDialog::ProfileDialog(QWidget* parent)
	:QDialog(parent)
{
	setWindowTitle("Profilul meu"); 
	setMinimumSize(750, 520); // seteaza dim minima, userul nu poate face fereastra mai mica de atat
    // avem nev ca sa nu se suprapuna widgeturile


	QVBoxLayout* mainLayout = new QVBoxLayout(this);
// layouturile in qt sunt sisteme care pozitioneaza automat widgeturile
// in loc sa fol manual coordonate ca in LoginDialog, folosim layouturi care se adapteaza automat la redimensionare
// la vertical layout - tot ce adaugam cu addWidget() apare unul sub celalalt
// this - e dialogul nostru


	// informatii cont

    QGroupBox* groupInfo = new QGroupBox("Informatii cont");
// qgroupbox e un container vizual cu un titlu (textul afisat in coltul din stanga sus al chenarului
// folosim ca sa grupam separat info contului de tabelul de rezervari
    QFormLayout* formLayout = new QFormLayout(groupInfo);
// qformlayout e un layout special pt formulare: are 2 coloane, stanga pt label, dreapta pt camp


// cream labeluri goale (fara text) pt ca textul real va fi completat dupa de functia incarcaDateUser()
// ..care citeste din baza de date. addrow() adauga automat labelul text pe stanga (Nume:) si widgtul nostru pe dreapta
// |------
    labelNume = new QLabel();
    labelPrenume = new QLabel();
    labelUsername = new QLabel();
    labelEmail = new QLabel();
    labelTelefon = new QLabel();

    formLayout->addRow("Nume:", labelNume);
    formLayout->addRow("Prenume:", labelPrenume);
    formLayout->addRow("Username:", labelUsername);
    formLayout->addRow("Email:", labelEmail);
    formLayout->addRow("Telefon:", labelTelefon);
// |_______

    mainLayout->addWidget(groupInfo);
// adaugam tot groupboxul cu info in layoutul principal


    // rezervarile mele

    QGroupBox* groupRezervari = new QGroupBox("Rezervarile mele");
    QVBoxLayout* rezervariLayout = new QVBoxLayout(groupRezervari);

    tableRezervari = new QTableWidget();
    tableRezervari->setColumnCount(8);

    QStringList headers;

    headers << "ID" << "Ruta" << "Data plecare" << "Data intoarcere"
        << "Transport" << "Tip loc" << "Nr. locuri" << "Pret total (RON)";

    tableRezervari->setHorizontalHeaderLabels(headers);

    tableRezervari->setEditTriggers(QAbstractItemView::NoEditTriggers);
// userul nu poate edita celulele direct in tabel(tabelul e read only)
// fara asta, userul ar putea da dublu click si modifica textul

    tableRezervari->setSelectionBehavior(QAbstractItemView::SelectRows);
// cand dai click pe o celula se selecteaza intregul rand, nu doar celula
// de ce? ca sa fie mai intuitiv ca selectam rezervarea cu totul

    tableRezervari->horizontalHeader()->setStretchLastSection(true);
// ultima coloana se intinde cat spatiu ramas are, ca sa evitam spatiul gol la dreapta tabelului

    rezervariLayout->addWidget(tableRezervari);

    butonAnuleaza = new QPushButton("Anuleaza rezervarea selectata");
    rezervariLayout->addWidget(butonAnuleaza);

    mainLayout->addWidget(groupRezervari);


    // buton inchidere

    butonInchide = new QPushButton("Inchide");
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(butonInchide);
    mainLayout->addLayout(btnLayout);

    connect(butonAnuleaza, &QPushButton::clicked,
        this, &ProfileDialog::on_anuleazaRezervareButton_clicked);
    connect(butonInchide, &QPushButton::clicked,
        this, &QDialog::accept);

// connect() e mecanismul principal de events in qt
// sintaxa: connect(sursa, &ClasaSursa::semnal, destinatie, &ClasaDest::slot)
    
// primul connect: cand se da click pe butonanuleaza, se apeleaza on_anuleazarezervarebutton_clicked()
// al doilea connect: butoninchide ->se apeleaza QDialog::accept(), o functie mostenita care 
//.. inchide dialogul si returneaza QDialog::Accepted

// dc &qpushbutton::clicked si nu clicked pur? qt 5+ fol sintaxa cu pointer-to-member-function pt
//.. type safety, adica compilatorul verifica la compilare ca semnalul si slotul au semnatura compatibila

    incarcaDateUser();
    incarcaRezervari();
// le apelam la sfarsit, dupa ce toate widgeturile au fost create pt ca ordinea conteaza
// nu poti completa un label daca nu exista
}




void ProfileDialog::incarcaDateUser()
{
    QSqlQuery query;

    query.prepare(  
        "SELECT nume, prenume, username, email, telefon "
        "FROM Utilizatori WHERE id_user = :id"
    );
// pregateste query ul cu placeholdere(:id). nu concatenam direct valoarea in string!
// adc nu scriem WHERE id_user = " + UserSession::id + " pt ca
    // 1 sql injection - un user malitios ar putea introduce cod sql prin valori
    // 2 qt face automat conversia din int/qstring la tipul sql corect
    // eficienta-queryul preparat poate fi re executat mai rapid


    query.bindValue(":id", UserSession::getInstance().id);
    query.exec();
// UserSession::id e un camp static din structura UserSession
// tine minte id ul userului logat curent pe tot parcursul sesiunii


    if (query.next()) {
        labelNume->setText(query.value("nume").toString());
        labelPrenume->setText(query.value("prenume").toString());
        labelUsername->setText(query.value("username").toString());
        labelEmail->setText(query.value("email").toString());
        labelTelefon->setText(query.value("telefon").toString());
    }
// query.next() incearca sa mearga la primul sau urmtorul rand din rezultat- returneaza true daca exista date
// pt ca cautam dupa PRIMARY KEY (id_user unic), va exista cel mult un rand

// query.value("nume") returneaza un QVariant- un tip universal qt care poate contine orice
// toString() il converteste la QString pt setText()
}



void ProfileDialog::incarcaRezervari()
{
    tableRezervari->setRowCount(0);
// stergem toate randurile existente inainte de a reincarca ca sa putem apela si dupa ce stergem o rezervare

    QSqlQuery query;
    query.prepare(
        "SELECT r.id_rezervare, o.oras_plecare, o.oras_destinatie, "
        "o.data_plecare, o.data_intoarcere, t.tip, cb.tip_clasa, "
        "r.numar_locuri, r.pret_total "
        "FROM Rezervari r "
        "JOIN Oferta o ON r.id_oferta = o.id_oferta "
        "JOIN Transporturi t ON o.id_transport = t.id_transport "
        "JOIN Categorii_Bilete cb ON r.id_categorie = cb.id_categorie "
        "WHERE r.id_user = :idUser "
        "ORDER BY r.data_rezervare DESC"
        // sortam descrescator dupa data rezervarii ca sa apara cele mai recente rezervari primele
    );

    query.bindValue(":idUser", UserSession::getInstance().id);
    query.exec();


    int row = 0;

    while (query.next()) {

        tableRezervari->insertRow(row);
        int idRezervare = query.value("id_rezervare").toInt();

        QString ruta = query.value("oras_plecare").toString()
            + " \u2192 "
            + query.value("oras_destinatie").toString();
// \u2192 e codul unicode pt sageata. il fol pt a afisa bucuresti → istanbul intr o singura coloana

        QTableWidgetItem* itemId = new QTableWidgetItem(QString::number(idRezervare));
// fiecare celula din tabel e un qtablewidgetitem. afisam id ul rezervarii ca text vizibil
        itemId->setData(Qt::UserRole, idRezervare);  // salvam id-ul ascuns pt stergere
//setData(...) stocheaza id ul ca date ascunse (int, nu string)
//qt::userrole e un slot rezervat pt date custom ale aplicatiei - nu interfereaza datele vizuale
// la stergere citim valoarea int direct din UserRole, nu mai trb sa convertim textul

        tableRezervari->setItem(row, 0, itemId);
        tableRezervari->setItem(row, 1, new QTableWidgetItem(ruta));
        tableRezervari->setItem(row, 2, new QTableWidgetItem(
            query.value("data_plecare").toDate().toString("yyyy-MM-dd")));
        tableRezervari->setItem(row, 3, new QTableWidgetItem(
            query.value("data_intoarcere").toDate().toString("yyyy-MM-dd")));
        tableRezervari->setItem(row, 4, new QTableWidgetItem(query.value("tip").toString()));
        tableRezervari->setItem(row, 5, new QTableWidgetItem(query.value("tip_clasa").toString()));
        tableRezervari->setItem(row, 6, new QTableWidgetItem(query.value("numar_locuri").toString()));
        tableRezervari->setItem(row, 7, new QTableWidgetItem(
            QString::number(query.value("pret_total").toDouble(), 'f', 2)));

        row++;
    }
}


void ProfileDialog::on_anuleazaRezervareButton_clicked()
{
    int currentRow = tableRezervari->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Atentie", "Selectati o rezervare din tabel pentru a o anula.");
        return;
    }
// currentRow() returneaza indexul randului selectat curent sau -1 daca nimic nu e selectat


    int idRezervare = tableRezervari->item(currentRow, 0)->data(Qt::UserRole).toInt();
// luam id ul rezervarii din coloana 0 (prima coloana) din datele ascunse (qt::UserRole)
// toInt() cnvertste qvariantul la int


    QMessageBox::StandardButton raspuns = QMessageBox::question(
        this, "Confirmare",
        "Esti sigur ca vrei sa anulezi aceasta rezervare?",
        QMessageBox::Yes | QMessageBox::No
    );
// qmessagebox::question() afiseaza o fereastra de confirmare cu butoane yes/no si returneaza butonul apasat
// | e operatorul bitwise OR - il folosim sa specificam ce butoane vrem sa apara
// e important sa stergem confirmare inainte de stergere


    if (raspuns != QMessageBox::Yes)
        return;

    QSqlQuery deleteQuery;
    deleteQuery.prepare(
        "DELETE FROM Rezervari WHERE id_rezervare = :id AND id_user = :idUser"
    );
// dc punem AND id_user=:idUser? pt securitate: chiar daca un user cunoaste id-ul rezervarii altcuiva,
//.. nu poate sterge decat rezervarile proprii. serverul sql va returna 0 randuri afectate daca id_user nu corespunde


    deleteQuery.bindValue(":id", idRezervare);
    deleteQuery.bindValue(":idUser", UserSession::getInstance().id);

    if (deleteQuery.exec()) {
        QMessageBox::information(this, "Succes", "Rezervarea a fost anulata cu succes.");
        incarcaRezervari(); // reincarcam tabelul ca sa dispara randul sters
    }
    else {
        QMessageBox::critical(this, "Eroare",
            "Eroare la anulare: " + deleteQuery.lastError().text());
    }
}