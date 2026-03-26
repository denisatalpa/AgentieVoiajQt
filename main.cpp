//#include <QtWidgets/QApplication>
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); //creeaza aplicatia qt
    //poate exista un singur qapplication in orice program qt

    MainWindow window;
    window.show();

    return app.exec();
}