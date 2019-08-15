#include "mainwindow.h"
#include "scriptmodel.h"
#include "vcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    vController the_controller;
    scriptmodel le_model(the_controller, w);
    the_controller = vController(le_model, w);
    w.addListener(the_controller);
    w.show();

    return a.exec();
}
