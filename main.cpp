#include <QCoreApplication>
#include <iostream>
#include "server.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "---- Server Started ---\n";
    Server myServer;
    //myServer.listen();

    return a.exec();
}
