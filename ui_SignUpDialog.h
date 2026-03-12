/********************************************************************************
** Form generated from reading UI file 'SignUpDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNUPDIALOG_H
#define UI_SIGNUPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SignUpDialog
{
public:
    QLabel *label;
    QLineEdit *numeEdit;
    QLabel *label_2;
    QLineEdit *prenumeEdit;
    QLabel *label_3;
    QLineEdit *usernameEdit;
    QLabel *label_4;
    QLineEdit *emailEdit;
    QLabel *label_5;
    QLineEdit *telefonEdit;
    QLabel *label_6;
    QLineEdit *parolaEdit;
    QPushButton *creeazaContButton;

    void setupUi(QDialog *SignUpDialog)
    {
        if (SignUpDialog->objectName().isEmpty())
            SignUpDialog->setObjectName("SignUpDialog");
        SignUpDialog->resize(400, 481);
        label = new QLabel(SignUpDialog);
        label->setObjectName("label");
        label->setGeometry(QRect(40, 10, 49, 16));
        numeEdit = new QLineEdit(SignUpDialog);
        numeEdit->setObjectName("numeEdit");
        numeEdit->setGeometry(QRect(40, 30, 113, 24));
        label_2 = new QLabel(SignUpDialog);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 60, 49, 16));
        prenumeEdit = new QLineEdit(SignUpDialog);
        prenumeEdit->setObjectName("prenumeEdit");
        prenumeEdit->setGeometry(QRect(40, 80, 113, 24));
        label_3 = new QLabel(SignUpDialog);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(40, 110, 71, 16));
        usernameEdit = new QLineEdit(SignUpDialog);
        usernameEdit->setObjectName("usernameEdit");
        usernameEdit->setGeometry(QRect(40, 130, 113, 24));
        label_4 = new QLabel(SignUpDialog);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(40, 160, 49, 16));
        emailEdit = new QLineEdit(SignUpDialog);
        emailEdit->setObjectName("emailEdit");
        emailEdit->setGeometry(QRect(40, 180, 113, 24));
        label_5 = new QLabel(SignUpDialog);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(40, 210, 81, 16));
        telefonEdit = new QLineEdit(SignUpDialog);
        telefonEdit->setObjectName("telefonEdit");
        telefonEdit->setGeometry(QRect(40, 230, 113, 24));
        label_6 = new QLabel(SignUpDialog);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(40, 290, 49, 16));
        parolaEdit = new QLineEdit(SignUpDialog);
        parolaEdit->setObjectName("parolaEdit");
        parolaEdit->setGeometry(QRect(40, 310, 113, 24));
        parolaEdit->setEchoMode(QLineEdit::EchoMode::Password);
        creeazaContButton = new QPushButton(SignUpDialog);
        creeazaContButton->setObjectName("creeazaContButton");
        creeazaContButton->setGeometry(QRect(40, 370, 80, 24));

        retranslateUi(SignUpDialog);

        QMetaObject::connectSlotsByName(SignUpDialog);
    } // setupUi

    void retranslateUi(QDialog *SignUpDialog)
    {
        SignUpDialog->setWindowTitle(QCoreApplication::translate("SignUpDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SignUpDialog", "Nume", nullptr));
        label_2->setText(QCoreApplication::translate("SignUpDialog", "Prenume", nullptr));
        label_3->setText(QCoreApplication::translate("SignUpDialog", "Username", nullptr));
        label_4->setText(QCoreApplication::translate("SignUpDialog", "Email", nullptr));
        label_5->setText(QCoreApplication::translate("SignUpDialog", "Nr. telefon", nullptr));
        label_6->setText(QCoreApplication::translate("SignUpDialog", "Parola", nullptr));
        creeazaContButton->setText(QCoreApplication::translate("SignUpDialog", "Creeaza Cont", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignUpDialog: public Ui_SignUpDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNUPDIALOG_H
