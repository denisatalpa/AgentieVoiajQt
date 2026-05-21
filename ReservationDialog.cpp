#include "ReservationDialog.h"
#include "UserSession.h"
#include "ServerConnection.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>

ReservationDialog::ReservationDialog(QWidget* parent, 
	int idOferta, const QString& plecare, const QString& destinatie, 
	const QString& dataPlecare, const QString& dataIntoarcere, 
	const QString& transport, double pretRegular, double pretPremium, 
	int locuriRegular, int locuriPremium)

	: QDialog(parent),
	m_idOferta(idOferta),
	m_pretRegular(pretRegular), m_pretPremium(pretPremium),
	m_locuriRegular(locuriRegular), m_locuriPremium(locuriPremium)

{
	setWindowTitle("Rezervare");
	setMinimumWidth(350);

	QVBoxLayout* layout = new QVBoxLayout(this);


	// Informatii oferta
	layout->addWidget(new QLabel(
		"Ruta: " + plecare + " \u2192 " + destinatie));
	layout->addWidget(new QLabel(
		"Plecare: " + dataPlecare +
		" | Intoarcere: " + dataIntoarcere));
	layout->addWidget(new QLabel("Transport: " + transport));
	layout->addWidget(new QLabel(
		"Locuri Regular disponibile: " +
		QString::number(locuriRegular) +
		" | Pret: " + QString::number(pretRegular, 'f', 2) + " RON"));
	layout->addWidget(new QLabel(
		"Locuri Premium disponibile: " +
		QString::number(locuriPremium) +
		" | Pret: " + QString::number(pretPremium, 'f', 2) + " RON"));


	// Tip loc
	layout->addWidget(new QLabel("Tip loc:"));
	comboTip = new QComboBox();
	comboTip->addItem("Regular");
	comboTip->addItem("Premium");
	layout->addWidget(comboTip);

	// Numar locuri
	layout->addWidget(new QLabel("Numar locuri:"));
	spinLocuri = new QSpinBox();
	spinLocuri->setMinimum(1);
	spinLocuri->setMaximum(locuriRegular > 0 ? locuriRegular : 1);
	layout->addWidget(spinLocuri);

	// Pret total
	labelPretTotal = new QLabel("Pret total: 0.00 RON");
	labelPretTotal->setStyleSheet("font-weight: bold;");
	layout->addWidget(labelPretTotal);


	// Butoane
	QHBoxLayout* btnLayout = new QHBoxLayout();
	QPushButton* butonRezerva = new QPushButton("Rezerva");
	QPushButton* butonAnuleaza = new QPushButton("Anuleaza");
	btnLayout->addWidget(butonRezerva);
	btnLayout->addWidget(butonAnuleaza);
	layout->addLayout(btnLayout);

	connect(comboTip, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &ReservationDialog::actualizeazaPret);
	connect(spinLocuri, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &ReservationDialog::actualizeazaPret);
	connect(butonRezerva, &QPushButton::clicked,
		this, &ReservationDialog::on_rezervaButton_clicked);
	connect(butonAnuleaza, &QPushButton::clicked,
		this, &QDialog::reject);

	actualizeazaPret();

	actualizeazaPret(); // calculam pretul initial
}


void ReservationDialog::actualizeazaPret()
{
	QString tip = comboTip->currentText();
	int nrLocuri = spinLocuri->value();

	double pret = (tip == "Regular") ? m_pretRegular : m_pretPremium;
	int maxLocuri = (tip == "Regular") ? m_locuriRegular : m_locuriPremium;

	spinLocuri->setMaximum(maxLocuri > 0 ? maxLocuri : 1);

	double total = pret * nrLocuri;
	labelPretTotal->setText("Pret total: " +
		QString::number(total, 'f', 2) + " RON");
}


void ReservationDialog::on_rezervaButton_clicked()
{
	if (!UserSession::getInstance().esteLogat) {
		QMessageBox::warning(this, "Eroare", "Nu esti logat.");
		return;
	}

	QString tip = comboTip->currentText();
	int nrLocuri = spinLocuri->value();
	int maxLocuri = (tip == "Regular") ? m_locuriRegular : m_locuriPremium;

	if (maxLocuri <= 0) {
		QMessageBox::warning(this, "Eroare",
			"Nu mai sunt locuri " + tip + " disponibile.");
		return;
	}

	double pret = (tip == "Regular") ? m_pretRegular : m_pretPremium;
	double total = pret * nrLocuri;

	// Trimitem comanda la server:
   // REZERVA|id_user|id_oferta|tip_clasa|nr_locuri|pret_total
	QString comanda = "REZERVA|" +
		QString::number(UserSession::getInstance().id) + "|" +
		QString::number(m_idOferta) + "|" +
		tip + "|" +
		QString::number(nrLocuri) + "|" +
		QString::number(total, 'f', 2);

	QStringList raspuns = ServerConnection::getInstance()
		.trimiteComanda(comanda);

	if (raspuns.isEmpty()) {
		QMessageBox::critical(this, "Eroare", "Fara raspuns de la server.");
		return;
	}

	QStringList parts = raspuns[0].split('|');

	if (parts[0] == "OK") {
		QMessageBox::information(this, "Succes",
			"Rezervarea a fost efectuata cu succes!");
		accept();
	}
	else {
		QMessageBox::critical(this, "Eroare",
			parts.size() > 1 ? parts[1] : "Rezervare esuata.");
	}
}