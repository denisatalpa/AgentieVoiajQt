#include "AdminDialog.h"
#include "ServerConnection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>

AdminDialog::AdminDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Panou Admin");
    setMinimumSize(800, 550);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);


    // ofertele existente
    QGroupBox* groupOferte = new QGroupBox("Oferte existente");
    QVBoxLayout* oferteLay = new QVBoxLayout(groupOferte);

    tableOferte = new QTableWidget();
    tableOferte->setColumnCount(7);
    tableOferte->setHorizontalHeaderLabels(
        { "ID", "Plecare", "Destinatie", "Data plecare",
         "Data intoarcere", "Transport", "Pret Regular" });

    tableOferte->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableOferte->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableOferte->horizontalHeader()->setStretchLastSection(true);
    oferteLay->addWidget(tableOferte);
    mainLayout->addWidget(groupOferte);


    // adaugare oferta noua
    QGroupBox* groupAdd = new QGroupBox("Adauga oferta noua");
    QFormLayout* addLayout = new QFormLayout(groupAdd);

    editPlecare = new QLineEdit();
    editDestinatie = new QLineEdit();
    editDataPlecare = new QLineEdit(); editDataPlecare->setPlaceholderText("YYYY-MM-DD");
    editDataIntoarcere = new QLineEdit(); editDataIntoarcere->setPlaceholderText("YYYY-MM-DD");
   
    comboTransport = new QComboBox();
    comboTransport->addItem("Autocar (id=1)", 1);
    comboTransport->addItem("Microbuz (id=2)", 2);
    
    editPret = new QLineEdit(); editPret->setPlaceholderText("ex: 250.00");

    addLayout->addRow("Oras plecare:", editPlecare);
    addLayout->addRow("Oras destinatie:", editDestinatie);
    addLayout->addRow("Data plecare:", editDataPlecare);
    addLayout->addRow("Data intoarcere:", editDataIntoarcere);
    addLayout->addRow("Transport:", comboTransport);
    addLayout->addRow("Pret standard:", editPret);

    butonAdauga = new QPushButton("Adauga oferta");
    addLayout->addRow(butonAdauga);
    mainLayout->addWidget(groupAdd);

    // modificare sau stergere
    QGroupBox* groupEdit = new QGroupBox("Modifica / Sterge oferta selectata");
    QHBoxLayout* editLayout = new QHBoxLayout(groupEdit);

    editPretNou = new QLineEdit(); editPretNou->setPlaceholderText("Pret nou standard");
   
    butonModifica = new QPushButton("Modifica pretul");
    butonSterge = new QPushButton("Sterge oferta");
    butonSterge->setStyleSheet("color: red;");

    editLayout->addWidget(new QLabel("Pret nou:"));
    editLayout->addWidget(editPretNou);
    editLayout->addWidget(butonModifica);
    editLayout->addStretch();
    editLayout->addWidget(butonSterge);
    mainLayout->addWidget(groupEdit);

    // pt inchidere
    butonInchide = new QPushButton("Inchide");
    QHBoxLayout* closeLay = new QHBoxLayout();
    closeLay->addStretch();
    closeLay->addWidget(butonInchide);
    mainLayout->addLayout(closeLay);

    connect(butonAdauga, &QPushButton::clicked,
        this, &AdminDialog::on_adaugaButton_clicked);
    connect(butonModifica, &QPushButton::clicked,
        this, &AdminDialog::on_modificaButton_clicked);
    connect(butonSterge, &QPushButton::clicked,
        this, &AdminDialog::on_stergeButton_clicked);
    connect(butonInchide, &QPushButton::clicked,
        this, &QDialog::accept);

    incarcaOferte();
}

void AdminDialog::incarcaOferte()
{
    tableOferte->setRowCount(0);
    QStringList linii = ServerConnection::getInstance()
        .trimiteComanda("GET_OFFERS");
    int row = 0;
    for (const QString& linie : linii) {
        QStringList p = linie.split('|');
        if (p.size() < 8) continue;
        tableOferte->insertRow(row);
        // p[0]=id, p[1]=plecare, p[2]=dest, p[3]=dataPl,
        // p[4]=dataIntoarcere, p[5]=transport, p[6]=pretRegular
        for (int col = 0; col < 7; col++)
            tableOferte->setItem(row, col,
                new QTableWidgetItem(p[col]));
        // stocam id ascuns
        tableOferte->item(row, 0)
            ->setData(Qt::UserRole, p[0].toInt());
        row++;
    }
}

void AdminDialog::on_adaugaButton_clicked()
{
    if (editPlecare->text().isEmpty() || editDestinatie->text().isEmpty() ||
        editDataPlecare->text().isEmpty() || editDataIntoarcere->text().isEmpty() ||
        editPret->text().isEmpty()) {
        QMessageBox::warning(this, "Atentie", "Completeaza toate campurile.");
        return;
    }

    int idTransport = comboTransport->currentData().toInt();

    // ADAUGA_OFERTA|plecare|destinatie|dataPl|dataIntoarcere|
    //              id_transport|pret_standard
    QString comanda = "ADAUGA_OFERTA|" +
        editPlecare->text() + "|" +
        editDestinatie->text() + "|" +
        editDataPlecare->text() + "|" +
        editDataIntoarcere->text() + "|" +
        QString::number(idTransport) + "|" +
        editPret->text();

    QStringList r = ServerConnection::getInstance().trimiteComanda(comanda);

    if (!r.isEmpty() && r[0].startsWith("OK")) {
        QMessageBox::information(this, "Succes", "Oferta adaugata.");
        // golim campurile
        editPlecare->clear(); editDestinatie->clear();
        editDataPlecare->clear(); editDataIntoarcere->clear();
        editPret->clear();
        incarcaOferte();
    }
    else {
        QMessageBox::critical(this, "Eroare", "Adaugare esuata.");
    }
}

void AdminDialog::on_stergeButton_clicked()
{
    int row = tableOferte->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Atentie",
            "Selectati o oferta din tabel.");
        return;
    }
    int id = tableOferte->item(row, 0)->data(Qt::UserRole).toInt();

    if (QMessageBox::question(this, "Confirmare",
        "Stergi oferta cu ID " + QString::number(id) + "?",
        QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    QStringList r = ServerConnection::getInstance()
        .trimiteComanda("STERGE_OFERTA|" +
            QString::number(id));

    if (!r.isEmpty() && r[0].startsWith("OK")) {
        QMessageBox::information(this, "Succes", "Oferta stearsa.");
        incarcaOferte();
    }
    else {
        QMessageBox::critical(this, "Eroare", "Stergere esuata.");
    }
}

void AdminDialog::on_modificaButton_clicked()
{
    int row = tableOferte->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Atentie",
            "Selectati o oferta din tabel.");
        return;
    }
    if (editPretNou->text().isEmpty()) {
        QMessageBox::warning(this, "Atentie",
            "Introdu pretul nou.");
        return;
    }

    int id = tableOferte->item(row, 0)->data(Qt::UserRole).toInt();

    // MODIFICA_OFERTA|id_oferta|pret_nou
    QString comanda = "MODIFICA_OFERTA|" +
        QString::number(id) + "|" + editPretNou->text();

    QStringList r = ServerConnection::getInstance()
        .trimiteComanda(comanda);

    if (!r.isEmpty() && r[0].startsWith("OK")) {
        QMessageBox::information(this, "Succes", "Pret actualizat.");
        editPretNou->clear();
        incarcaOferte();
    }
    else {
        QMessageBox::critical(this, "Eroare", "Modificare esuata.");
    }
}