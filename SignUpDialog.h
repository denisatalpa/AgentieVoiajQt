#pragma once

#include <QDialog>


namespace Ui {
	class SignUpDialog;
}


class SignUpDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SignUpDialog(QWidget* parent = nullptr);
	~SignUpDialog();

private slots:
	void on_creeazaContButton_clicked();

private:
	Ui::SignUpDialog* ui;
};

