#pragma once
#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>


class ReservationDialog : public QDialog
{
	Q_OBJECT

public:
    ReservationDialog(QWidget* parent,
        int idOferta,
        const QString& plecare,
        const QString& destinatie,
        const QString& dataPlecare,
        const QString& dataIntoarcere,
        const QString& transport,
        double pretRegular,
        double pretPremium,
        int locuriRegular,
        int locuriPremium);
// de ce const qstring& in loc de qstring? pt ca const garanteaza ca nu modificam valoarea (read only)
// & referinta- nu se face o copie a stringului, se transmite direct aa (mai eficient)


private slots:
    void actualizeazaPret();
    void on_rezervaButton_clicked();

private:
    int m_idOferta;
    double m_pretRegular;
    double m_pretPremium;
    int m_locuriRegular;
    int m_locuriPremium;
// al fol m_ ca o conventie pt membrii clasei pt a sti imd ca este un camp al clasei

    QLabel* labelInfo;
    QComboBox* comboTip;
    QSpinBox* spinLocuri;
    QLabel* labelPretTotal;
    QPushButton* butonRezerva;
    QPushButton* butonAnuleaza;
};

