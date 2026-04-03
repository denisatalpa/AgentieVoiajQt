#pragma once

#include <QDialog> // qdialog e o fereastra de tip popup

// qt desigener genereaza automat un fisier ui_SignUpDialog.h
// unde exista o clasa "class Ui::SignUpDIalog care contine toate widgeturile
// widgeturile din UI (QLineEdit *usernameEdit etc)
// noi aici doar declaram ca exista, fara sa o definim <=> forward declaration

namespace Ui { 
	class SignUpDialog;
// forward declaration -in loc sa includem tot ui_SignUpDialog.h in header, spunem doar 
//.. exista o clasa numita Ui::SignUpDialog
// avantaj: fisierele care includ SignUpDialog nu trb sa stie nimic despre ui => compileaza mai rpd
}


class SignUpDialog : public QDialog // SignUpDialog devine o fereastra qt
{
	Q_OBJECT // o macrodefinitie obligatorie in qt pt orice clasa care fol semnale
// si sloturi(sistemul de evenimente al qt) fara ea, sloturile noastre nu ar functiona


public:
	explicit SignUpDialog(QWidget* parent = nullptr); // mainwindow este parent
// explicit = previne conversiile implicite. fara asta, compilatorul ar putea converti 
//.. automat un qwidget* la signupdialog* in anumie contexte

// dc folosim parent? 1.pt managementul memoriei: qt sterge automat obiectele copil
// adica daca mainwindow este distrus, qt distruge automat si signupdialog si logindialog
// parent = nullptr -> fereastra poate exista si fara o fereastra parinte
	
	~SignUpDialog();

private slots: // slot = o functie care se executa ca raspuns la un eveniment
	void on_creeazaContButton_clicked();
	// conventia on_numeobiect_numeeveniment e recunoscuta automat de qt

private:
	Ui::SignUpDialog* ui;
	// un pointer catre obiectul ui generat automat de qt din fisierul .ui
	// contine toate widgeturile din ui(ex: ui->usernameEdit)
};

