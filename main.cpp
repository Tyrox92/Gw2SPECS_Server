// main.cpp

#include <QApplication>
#include "myserver.h"

QTime timeOut1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Make a server and starts it
    MyServer server;
    server.startServer();

    return a.exec();

}
