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
    if (authvalue == 0)
    {
        authcode = qrand() % 89999 + 10001;
    } else {
        authcode = authvalue;
    }
    timeOut1.start();
    std::cout << "Gw2SPECS Server V1.1 Made by Tyrox and Kozzy\r\n\r\n";
    std::cout << "To use the global reset, do as follows:\r\n1. Right click anywhere in the client while being connected to the server\r\n2. Press \"Authenticate\"\r\n3. Enter the AuthCode and confirm\r\n\r\n";
    std::cout << "AuthCode: " << authcode << "\r\n";

    if (portvalue == 0)
    {
        std::cout << "Port (default=1234): ";
        if (std::cin.peek() == '\n') {
            port = 1234;
        } else if (!(std::cin >> port)) {
            std::cout << "Invalid input.\n";
        }
        if (port==0) port=1234;
    } else {
        port = portvalue;
    }

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
