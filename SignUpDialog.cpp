#include "SignUpDialog.h"
#include "ui_SignUpDialog.h"
#include <QMessageBox>
#include <QDebug>

SignUpDialog::SignUpDialog(QWidget* parent)
	:QDialog(parent), ui(new Ui::SignUpDialog)
{
	ui->setupUi(this); // leaga UI ul
}

SignUpDialog::~SignUpDialog()
{
	delete ui;
}

void SignUpDialog::on_creeazaContButton_clicked()
{
	QString nume = ui->numeEdit->text();
}
