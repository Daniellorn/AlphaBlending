#include "mainwindow.h"

#include <QApplication>

#include "ekran.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    Ekran ekran;
    ekran.show();
    return a.exec();
}
