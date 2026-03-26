#include "ReservationDialog.h"
#include "UserSession.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

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

	// informatii oferta
	labelInfo = new QLabel(
		QString("Ruta: %1 → %2\n"
			"Plecare: %3 | Intoarcere: %4\n"
			"Transport: %5\n"
			"Locuri Regular disponibile: %6 | Pret: %7 RON\n"
			"Locuri Premium disponibile: %8 | Pret: %9 RON")
		.arg(plecare).arg(destinatie)
		.arg(dataPlecare).arg(dataIntoarcere)
		.arg(transport)
		.arg(locuriRegular).arg(pretRegular, 0, 'f', 2)
		.arg(locuriPremium).arg(pretPremium, 0, 'f', 2)
	);

	layout->addWidget(labelInfo);

	// tip loc
	QLabel* labelTip = new QLabel("Tip loc:");
	comboTip = new QComboBox();

	comboTip->addItem("Regular");
	comboTip->addItem("Premium");

	layout->addWidget(labelTip);
	layout->addWidget(comboTip);


	// numar locuri
	QLabel* labelNr = new QLabel("Numar locuri:");
	spinLocuri = new QSpinBox();

	spinLocuri->setMinimum(1);
	spinLocuri->setMaximum(locuriRegular); // implicit pt Regular
	
	layout->addWidget(labelNr);
	layout->addWidget(spinLocuri);


	// pret total
	labelPretTotal = new QLabel("Pret total: 0.00 RON");
	labelPretTotal->setStyleSheet("font-weight: bold; font-size: 14px;");
	layout->addWidget(labelPretTotal);

	// butoane
	QHBoxLayout* btnLayout = new QHBoxLayout();
	butonRezerva = new QPushButton("Rezerva");
	butonAnuleaza = new QPushButton("Anuleaza");
	btnLayout->addWidget(butonRezerva);
	btnLayout->addWidget(butonAnuleaza);
	layout->addLayout(btnLayout);

	// conectam semnalele
	connect(comboTip, &QComboBox::currentTextChanged, this, &ReservationDialog::actualizeazaPret);
	connect(spinLocuri, QOverload<int>::of(&QSpinBox::valueChanged), this, &ReservationDialog::actualizeazaPret);
	connect(butonRezerva, &QPushButton::clicked, this, &ReservationDialog::on_rezervaButton_clicked);
	connect(butonAnuleaza, &QPushButton::clicked, this, &QDialog::reject);

	actualizeazaPret(); // calculam pretul initial
}


void ReservationDialog::actualizeazaPret()
{
	QString tip = comboTip->currentText();
	int nrLocuri = spinLocuri->value();

	// actualizam maximul in functie de tipul ales
	if (tip == "Regular")
		spinLocuri->setMaximum(m_locuriRegular > 0 ? m_locuriRegular : 1);
	else
		spinLocuri->setMaximum(m_locuriPremium > 0 ? m_locuriPremium : 1);

	double pret = (tip == "Regular") ? m_pretRegular : m_pretPremium;
	double total = pret * nrLocuri;

	labelPretTotal->setText(QString("Pret total: %1 RON").arg(total, 0, 'f', 2));
}


void ReservationDialog::on_rezervaButton_clicked()
{
	QString tip = comboTip->currentText();
	int     nrLocuri = spinLocuri->value();
	double  pret = (tip == "Regular") ? m_pretRegular : m_pretPremium;
	double  total = pret * nrLocuri;

	// verificam locuri disponibile
	if (tip == "Regular" && nrLocuri > m_locuriRegular) {
		QMessageBox::warning(this, "Eroare", "Nu sunt suficiente locuri Regular!");
		return;
	}
	if (tip == "Premium" && nrLocuri > m_locuriPremium) {
		QMessageBox::warning(this, "Eroare", "Nu sunt suficiente locuri Premium!");
		return;
	}

	// gasim id_categorie in functie de tip
	QSqlQuery catQuery;
	catQuery.prepare(
		"SELECT cb.id_categorie FROM Categorii_Bilete cb "
		"JOIN Transporturi t ON cb.id_transport = t.id_transport "
		"JOIN Oferta o ON o.id_transport = t.id_transport "
		"WHERE o.id_oferta = :idOferta AND cb.tip_clasa = :tip"
	);
	catQuery.bindValue(":idOferta", m_idOferta);
	catQuery.bindValue(":tip", tip);
	catQuery.exec();

	if (!catQuery.next()) {
		QMessageBox::critical(this, "Eroare", "Eroare la gasirea categoriei!");
		return;
	}

	int idCategorie = catQuery.value("id_categorie").toInt();

	// inseram rezervarea
	QSqlQuery insertQuery;
	insertQuery.prepare(
		"INSERT INTO Rezervari (id_user, id_oferta, id_categorie, numar_locuri, pret_total) "
		"VALUES (:idUser, :idOferta, :idCategorie, :nrLocuri, :pretTotal)"
	);
	insertQuery.bindValue(":idUser", UserSession::id);
	insertQuery.bindValue(":idOferta", m_idOferta);
	insertQuery.bindValue(":idCategorie", idCategorie);
	insertQuery.bindValue(":nrLocuri", nrLocuri);
	insertQuery.bindValue(":pretTotal", total);

	if (insertQuery.exec()) {
		QMessageBox::information(this, "Succes",
			QString("Rezervare facuta cu succes!\nPret total: %1 RON").arg(total, 0, 'f', 2));
		accept();
	}
	else {
		QMessageBox::critical(this, "Eroare", "Eroare la rezervare: " + insertQuery.lastError().text());
	}
}