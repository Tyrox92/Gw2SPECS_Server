// main.cpp

#include <QCoreApplication>
#include "myserver.h"

QTime timeOut1;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Make a server and starts it
    MyServer server;
    server.startServer();

    return a.exec();

}
