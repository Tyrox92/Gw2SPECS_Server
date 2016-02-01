// mythread.cpp
#include <iostream>
#include <string>
#include "mythread.h"
#include <QtNetwork>

QByteArray ClientsData[10];
int ClientsIDs[10];
int NumberOfClients;



MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}


void MyThread::run()
{
    // thread starts here
    //qDebug() << " Thread started";

    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.


    int i;
    i=0;
    if (NumberOfClients<=10)  //max 10 clients for raid
    {
     while ((i<10) && (ClientsIDs[i]!=0)) i++;
     ClientsIDs[i]=socketDescriptor;
     //semi-handshake
     socket->write("***");
     socket->write(QByteArray::number(i));
     NumberOfClients++;
     qDebug() << "Client" << i+1 << "(" << socket->peerAddress().toString() << "-" << socketDescriptor << ") connected";
     qDebug() << NumberOfClients << " clients connected";
     connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
     connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

     // We'll have multiple clients, we want to know which is which
     //qDebug() << socketDescriptor << " Client connected";

     // make this thread a loop,
     // thread will stay alive so that signal/slot to function properly
     // not dropped out in the middle when thread dies

     exec();
    }
    //exit thread if more then 10 clients
}

void MyThread::readyRead()
{
    // get the information

    QByteArray Data = socket->readAll();
    int i;
    i=0;
    while ((i<10) && (ClientsIDs[i]!=socketDescriptor)) i++;
    if (i<10)
    {
        ClientsData[i]=Data;
    }
    for(i=0;i<10;i++)
    {
    if (ClientsIDs[i]>10)  socket->write(ClientsData[i]);
    if ((ClientsIDs[i]<10) && (ClientsIDs[i]>0))
        {
        sprintf(tmp1, "*%u1#Disconnected*%u2#0*%u3#0*%u4#0*", i,i,i,i);
        socket->write(tmp1);
        ClientsIDs[i]--;
        }
    if (ClientsIDs[i]<0) ClientsIDs[i]=0; //its free again
    }

}

void MyThread::disconnected()
{
    int i;
    i=0;
    while ((i<10) && (ClientsIDs[i]!=socketDescriptor)) i++;
    qDebug() << "Client" << i+1 << "(" << socket->peerAddress().toString() << "-" << socketDescriptor << ") disconnected";
    NumberOfClients--;
    qDebug() << NumberOfClients << " clients connected";
    ClientsIDs[i]=NumberOfClients; //disconnected not free yet
    socket->deleteLater();
    exit(0);
}
