//#include "mainwindow.h"
#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    Client c;
    QObject::connect(&c, &Client::LOGIN, &c, &Client::login);
    c.start_run(QString("zach"), QString("1"));
    return a.exec();
}
