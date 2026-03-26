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

private slots:
    void actualizeazaPret();
    void on_rezervaButton_clicked();

private:
    int m_idOferta;
    double m_pretRegular;
    double m_pretPremium;
    int m_locuriRegular;
    int m_locuriPremium;

    QLabel* labelInfo;
    QComboBox* comboTip;
    QSpinBox* spinLocuri;
    QLabel* labelPretTotal;
    QPushButton* butonRezerva;
    QPushButton* butonAnuleaza;
};

