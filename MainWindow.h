#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVector>

#include "ui_MainWindow.h"
#include "DialogFactory.h"
#include "OfertaInfo.h"

class MainWindow : public QMainWindow // preia toate functionalitatile unei ferestre principale qt
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	//sloturi conectate automat la butoane prin conventia de nume
	void on_loginButton_clicked();
	void on_signupButton_clicked();

	//o functie normala, nu e slot
	void incarcaOferte();

	void actualizeazaStatus(); // update labelul din stanga sus
	void onOfertaSelectata(int row, int col); // click pe oferta
	void deschideProfilulMeu();

private:
	Ui::MainWindowClass ui;
	// aici ui e obiect direct, nu e pointer. de aceea in mainwindow folosim ui.ceva, iar in 
	// signupdialog sau logindialog folosim ui->ceva
// DE CE?
// in mainwindow, qt visual studio wizard a generat automat obiect direct, asa ca l am pastrat

	QLabel* labelStatus; // guest sau logat ca: X
	QPushButton* butonProfilulMeu; // "contul meu"

	// lista de oferte incarcate dn bd (folosita si cu operatorii din ofertainfo)
	QVector<OfertaInfo> m_oferte;

	// factory methof, fiecare factory stie sa creeze un anumit tip de dialog
	LoginDialogFactory m_loginFactory;
	SignUpDialogFactory  m_signupFactory;
	ProfileDialogFactory m_profileFactory;
};

