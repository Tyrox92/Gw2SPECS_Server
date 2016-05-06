// myserver.cpp

#include "myserver.h"
#include "mythread.h"
#include <iostream>
#include <global.h>


MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer()
{
    QTime now = QTime::currentTime();
    qsrand(now.msec());

    int port = 0;
    authcode = qrand() % 89999 + 10001;
    timeOut1.start();
    std::cout << "Gw2SPECS Server V1.1 Made by Tyrox and Kozzy\r\n";
    std::cout << "Reset code: " << authcode << "\r\n";
    std::cout << "Port : (default=1234) ";
    if (std::cin.peek() == '\n') {
        port = 1234;
    } else if (!(std::cin >> port)) {
        std::cout << "Invalid input.\n";
    }
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
