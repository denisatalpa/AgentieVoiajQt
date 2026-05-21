#pragma once
#include <QDialog>
#include <QWidget>

// creator abstract- defineste interfata pt crearea dialogurilor
// subclasele decid ce tip de dialog se creeaza (factory method)
// mainwindow nu stie nimic despre tipul concret al dialogului, doar 
// apeleaza deschidedialog() si primeste rezultatul


class DialogFactory
{
public:
	virtual ~DialogFactory() = default;

	// factory method-metoda pura pe care fiecare subclasa o implementeaza
	// returneaza un dialog creat dinamic (cu new)

	virtual QDialog* creeazaDialog(QWidget* parent) const = 0;

	// metoda template care FOLOSESTE factory method ul
	// deschide dialogul modal si returneaza rezultatul (accepted/rejected)
	// sterge automat dialogul dupa inchidere

	int deschideDialog(QWidget* parent) const;
};

// concrete creators

class LoginDialogFactory :public DialogFactory
{
public:
	QDialog* creeazaDialog(QWidget* parent) const override;
};

class SignUpDialogFactory : public DialogFactory
{
public:
	QDialog* creeazaDialog(QWidget* parent) const override;
};

class ProfileDialogFactory : public DialogFactory
{
public:
	QDialog* creeazaDialog(QWidget* parent) const override;
};