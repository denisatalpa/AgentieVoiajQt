#include "DialogFactory.h"
#include "LoginDialog.h"
#include "SignUpDialog.h"
#include "ProfileDialog.h"


int DialogFactory::deschideDialog(QWidget* parent) const
{
    QDialog* dialog = creeazaDialog(parent); // apeleaza factory method
    int result = dialog->exec(); // deschide modal
    delete dialog; // curatam memoria
    return result;
}

QDialog* LoginDialogFactory::creeazaDialog(QWidget* parent) const
{
    return new LoginDialog(parent);
}

QDialog* SignUpDialogFactory::creeazaDialog(QWidget* parent) const
{
    return new SignUpDialog(parent);
}

QDialog* ProfileDialogFactory::creeazaDialog(QWidget* parent) const
{
    return new ProfileDialog(parent);
}
