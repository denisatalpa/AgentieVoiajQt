#pragma once
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>

class AdminDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AdminDialog(QWidget* parent = nullptr);

private slots:
    void on_adaugaButton_clicked();
    void on_stergeButton_clicked();
    void on_modificaButton_clicked();

private:
    void incarcaOferte();

    QTableWidget* tableOferte;

    // pt adaugare oferta noua
    QLineEdit* editPlecare;
    QLineEdit* editDestinatie;
    QLineEdit* editDataPlecare;
    QLineEdit* editDataIntoarcere;
    QComboBox* comboTransport;
    QLineEdit* editPret;

    // pt modificare pret
    QLineEdit* editPretNou;

    QPushButton* butonAdauga;
    QPushButton* butonSterge;
    QPushButton* butonModifica;
    QPushButton* butonInchide;
};