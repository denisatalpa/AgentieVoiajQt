#include "ProfileDialog.h"
#include "UserSession.h"
#include "ServerConnection.h" 

#include <QVBoxLayout> // pt layout vertical
#include <QHBoxLayout> // pt layout orizontal
#include <QFormLayout> // pt layout de tip formular (labelul in stanga si campul in dreapta)
#include <QGroupBox> // un dreptunghi cu titlu, ca sa grupeze vizual widgeturile
#include <QMessageBox> // fereastra popup ok/cancel/yes/no
#include <QHeaderView> // headerul tabelului 
#include <QDate> // tip de data calendaristica in qt





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
    // GET_PROFIL|id_user
    // Server raspunde: OK|nume|prenume|username|email|telefon
    QStringList raspuns = ServerConnection::getInstance()
        .trimiteComanda("GET_PROFIL|" +
            QString::number(UserSession::getInstance().id));

    if (raspuns.isEmpty()) return;
    QStringList p = raspuns[0].split('|');
    if (p[0] != "OK" || p.size() < 6) return;

    labelNume->setText(p[1]);
    labelPrenume->setText(p[2]);
    labelUsername->setText(p[3]);
    labelEmail->setText(p[4]);
    labelTelefon->setText(p[5]);
}



void ProfileDialog::incarcaRezervari()
{
    tableRezervari->setRowCount(0);

    // GET_REZERVARI|id_user
    // Fiecare linie: id|plecare|destinatie|dataPlecare|dataIntoarcere|
    //                transport|tip_clasa|nr_locuri|pret_total
    QStringList linii = ServerConnection::getInstance()
        .trimiteComanda("GET_REZERVARI|" +
            QString::number(UserSession::getInstance().id));

    int row = 0;
    for (const QString& linie : linii) {
        QStringList p = linie.split('|');
        if (p.size() < 9) continue;

        tableRezervari->insertRow(row);

        QString ruta = p[1] + " \u2192 " + p[2];

        QTableWidgetItem* itemId = new QTableWidgetItem(p[0]);
        itemId->setData(Qt::UserRole, p[0].toInt()); // id ascuns

        tableRezervari->setItem(row, 0, itemId);
        tableRezervari->setItem(row, 1, new QTableWidgetItem(ruta));
        tableRezervari->setItem(row, 2, new QTableWidgetItem(p[3]));
        tableRezervari->setItem(row, 3, new QTableWidgetItem(p[4]));
        tableRezervari->setItem(row, 4, new QTableWidgetItem(p[5]));
        tableRezervari->setItem(row, 5, new QTableWidgetItem(p[6]));
        tableRezervari->setItem(row, 6, new QTableWidgetItem(p[7]));
        tableRezervari->setItem(row, 7, new QTableWidgetItem(p[8]));
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


    if (QMessageBox::question(this, "Confirmare",
        "Esti sigur ca vrei sa anulezi aceasta rezervare?",
        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;
// qmessagebox::question() afiseaza o fereastra de confirmare cu butoane yes/no si returneaza butonul apasat
// | e operatorul bitwise OR - il folosim sa specificam ce butoane vrem sa apara
// e important sa stergem confirmare inainte de stergere


// CANCEL_REZERVARE|id_rezervare|id_user
    QString comanda = "CANCEL_REZERVARE|" +
        QString::number(idRezervare) + "|" +
        QString::number(UserSession::getInstance().id);

    QStringList raspuns = ServerConnection::getInstance()
        .trimiteComanda(comanda);

    if (!raspuns.isEmpty() && raspuns[0].startsWith("OK")) {
        QMessageBox::information(this, "Succes",
            "Rezervarea a fost anulata.");
        incarcaRezervari();
    }
    else {
        QMessageBox::critical(this, "Eroare", "Anulare esuata.");
    }
}