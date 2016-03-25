// myserver.cpp

#include "myserver.h"
#include "mythread.h"
#include <iostream>




MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer()
{
    int port = 0;
    timeOut1.start();
    std::cout << "GW2DPS Server V1.1 Made by Tyrox and Kozzy\n";
    std::cout << "Port : (default=1234) ";
    std::cin >> port;
    if (port==0) port=1234;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    MyThread *thread = new MyThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
