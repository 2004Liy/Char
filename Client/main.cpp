//#include "mainwindow.h"

#include <QApplication>
#include"form.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Form form;
    form.show();
    // MainWindow w;
    // w.show();
    return a.exec();
}
