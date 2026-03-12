#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_loginButton_clicked();
	void on_signupButton_clicked();

	void incarcaOferte();

private:
	Ui::MainWindowClass ui;
};

