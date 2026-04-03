//#include <QtWidgets/QApplication>

// in aplicatiile qt, mainul are un rol mic: porneste frameworkul si afiseaza fereastra principala

#include <QApplication> // clasa qt care initializeaza intreaga alicatie
//fara ea, niciun widget qt nu poate functiona
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); //creeaza aplicatia qt
    //poate exista un singur obiect qapplication in orice program qt
    // daca am crea doua, aplicatia ar crapa

    MainWindow window;
    window.show();
    // show il face vizibil. fara aceasta linie fereastra e creata dar nu apare pe ecran

    return app.exec();
    // app.exec() porneste bucla de evenimente a qt. aceasta bucla:
    // - asteapta evenimente(click mouse, tastatura, resize fereastra
    // - se executa infinit pana cand userul inchide fereastra principala
    // fara app.exec, programul ar afisa fereastra o fractiune de secunda si s-ar inchide imd
}