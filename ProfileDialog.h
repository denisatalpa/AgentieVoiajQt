#pragma once
#include <QDialog> // e clasa qt de baza pt ferestrele popup
#include <QLabel>
#include <QTableWidget>
#include <QPushButton>


class ProfileDialog : public QDialog
	// vrem sa folosim profiledialog exact ca pe un qdialog
{
	Q_OBJECT // macrodefinitie obligatorie in orice clasa qt care fol semnale si sloturi
// ea spune framework ului qt sagenereze cod suplimentar care activeaza sistemul de events al qt
// fara q_object, semnalele si sloturile nu functioneaza, iar connect() nu compileaza

public:
	explicit ProfileDialog(QWidget* parent = nullptr);
// parent in cazul nostru e MainWindow
// dc parent? qt fol un sistem de ownership: daca mainwindow e parintele dialogului,
//.. qt sterge automat dialogul cand sterge mainwindow si nu mai avem memory leak

// explicit = nu se poate face conversie implicita
// adica nu poti scrie din greseala ProfileDialog p = altceva
// =nullptr adica parametrul e optional, poti apela ProfileDialog() fara argumente

private slots: // un slot e o functie care poate fi conectata la semnal
	// adica se apeleaza automat cand se executa un eveniment

	void on_anuleazaRezervareButton_clicked();
// conventia on_numeObiect_numeEveniment() e recunoscuta autmat de qt daca folosim 
// .. QMetaObject::connectSlotsByName(). pt noi nu e necesar pt ca conectam cu connect(),
//.. dar e good practice

private:
	void incarcaDateUser();
	void incarcaRezervari();

	QLabel* labelNume;
	QLabel* labelPrenume;
	QLabel* labelUsername;
	QLabel* labelEmail;
	QLabel* labelTelefon;

	QTableWidget* tableRezervari;
	QPushButton* butonAnuleaza;
	QPushButton* butonInchide;

// toti membrii vizuali sunt declarati ca pointeri. dc pointeri si nu obiecte directe?
// 1 widgeturile qt sunt create dinamic cu new si gestionate de qt prin sistemul de parinti
// 2 qt sterge automat widgeturile child cand e distrus parintele
// 3 daca am folosi obiecte directe, destructorul ar fi apelat de 2 ori, o data de noi o data de qt -> crash
};

