/**
 * Radit - Radio Automation
 * -----------------------------------------
 *
 * Radit is a free software project to automate radio stations,
 * compatible with GNU/Linux, Windows, the project is in development.
 *
 * Site: www.radit.org
 *
 * @author Victor Algaba
 **/
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    w.showMaximized();
    return a.exec();
}
