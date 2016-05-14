// mythread.cpp
#include <iostream>
#include <string>
#include "mythread.h"
#include "myserver.h"
#include <QtNetwork>
#include <global.h>



QByteArray ClientsData[10];
int ClientsIDs[10];
int NumberOfClients;
int ClientsMask[10];
long ClientTimeOut[10];
long ClientLastTime[10];


MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}


void MyThread::run()
{

    socket = new QTcpSocket();


    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    // connect socket and signal
    // note - Qt::DirectConnection is used because it's multithreaded
    //        This makes the slot to be invoked immediately, when the signal is emitted.


    int i,j;
    i=0;
    j=10;
    for (i=0;i<10;i++)
     {
        if ((ClientsIDs[i]==1) && (timeOut1.elapsed()-ClientTimeOut[i]>6000))
            {
             qDebug() << "Cleaning timeouted Client" <<i;
             ClientsIDs[i]=0;NumberOfClients--;
             if (i<j) j=i;
            }
     }

    if (NumberOfClients<10)
    {
     i=0;
     while ((i<10) && (ClientsIDs[i]!=0)) i++;
     ClientsIDs[i]=socketDescriptor;
     ClientLastTime[i]=timeOut1.elapsed();
     socket->write("***");
     socket->write(QByteArray::number(i));
     NumberOfClients++;
     qDebug() << "Client" << i << " (" << socket->peerAddress().toString() << "-" << socketDescriptor << ") connected";
     qDebug() << NumberOfClients << " client(s) connected";
     connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
     connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

     // We'll have multiple clients, we want to know which is which

     // make this thread a loop,
     // thread will stay alive so that signal/slot to function properly
     // not dropped out in the middle when thread dies

     exec();
    }
    else
        {
           qDebug() << "Server is full: 10/10 clients connected";exit(0);
        }
}

void MyThread::readyRead()
{
    // get the information
    QByteArray Data = socket->readAll();
    int i,j,k;
    int resetCase = 0;
    int clientWhoTries = 0;
    i=0;j=0;

    if(connectTries == 0){
        if(Data == "1.0"){
            qDebug()<< "Client is on current Version. Access permitted...";
            connectTries = 1;
        }else{
            qDebug()<< "Old Version detected. Permission denied.";
            disconnected();
        }
    }else if(connectTries == 1){
        while ((i<10) && (ClientsIDs[i]!=socketDescriptor)) i++;

        if (i<10)
        {
            j=i;
            ClientsData[i]=Data;
            ClientLastTime[i]=timeOut1.elapsed();
        }
        else {
            qDebug() << "Incoming data error : NO MATCHING CLIENT ID";
        }

        for(i=0;i<10;i++)
        {
            // echo auth for reset request
            if (ClientsData[i][1]=='r' && ClientsData[i][2]=='e' && ClientsData[i][3]=='s')
            {
                clientWhoTries = i;
                QString authcode2(ClientsData[i].mid(4,ClientsData[i].indexOf("|",1)-4));
                if (authcode2.toInt() == authcode)
                {
                    resetCase = 1;

                } else {
                    resetCase = 2;
                }
            } else if (ClientsData[i][1]=='a' && ClientsData[i][2]=='d' && ClientsData[i][3]=='m' && ClientsData[i][4]=='i' && ClientsData[i][5]=='n'){
                qDebug()<< "Client" << i << "is trying to authenticate as Admin...";

                clientWhoTries = i;
                QString authcode3(ClientsData[i].mid(6,ClientsData[i].indexOf("|",1)-6));
                if (authcode3.toInt() == authcode)
                {
                    resetCase = 3;

                } else {
                    resetCase = 4;
                }
            }
            //qDebug() << "ClientsData["<<i<<"]" << ClientsData[i];
        }

        switch (resetCase)
        {
        case 1:
            // reset accepted
            socket->write("RES");
            qDebug() << "Reset Message from Client" << clientWhoTries << "has been sent.";
            break;
        case 2:
            // reset denied
            qDebug() << "Reset request from Client" << clientWhoTries << "has been denied; wrong AuthCode.";
            break;
        case 3:
            // Authentication successfull
            authSuccess = "ACK" + QString::number(authcode);
            tempAuth1 = authSuccess.toUtf8();
            tempAuth2 = tempAuth1.data();
            sprintf(writeAuth, "%s", tempAuth2);
            socket->write(writeAuth);
            qDebug() << "Client " << clientWhoTries << "has been accepted as Admin.";
            break;
        case 4:
            // Authentication denied
            qDebug() << "Client" << clientWhoTries << " has not been accepted - Reason: Wrong AuthCode.";
            break;
        default:
            qDebug() << "Default Case has been invoked. Why?";
        case 0:
            // echo player data
            for(i=0;i<10;i++)
            {
                if ((ClientsIDs[i]>1) &&  (timeOut1.elapsed()-ClientLastTime[i]>5000))
                {
                ClientsIDs[i]=1; //disconnected not free yet
                ClientsMask[i]=0;
                for (k=0;k<10;k++)
                    {
                     if (ClientsIDs[k]>1) ClientsMask[i]|=(1<<k);
                    }
                ClientTimeOut[i]=timeOut1.elapsed();
                }
                if (ClientsIDs[i]>1)
                {
                    socket->write(ClientsData[i]);
                }
                if (ClientsIDs[i]==1)
                {
                    if (timeOut1.elapsed()-ClientTimeOut[i]>3000)
                    {
                        ClientsIDs[i]=0;
                        NumberOfClients--;
                        qDebug() << "Client"<<i<< " timeout reached, fully disconnected";
                        qDebug() << NumberOfClients << " client(s) connected";
                    }
                    else
                    {
                        ClientsMask[i]&=  (~ (1<<j));
                        if (ClientsMask[i]==0)
                        {
                            ClientsIDs[i]=0;
                            NumberOfClients--;
                            qDebug() << "Client"<<i<< " disconnection mask reached zero, fully disconnected";
                            qDebug() << NumberOfClients << " client(s) connected";
                        }
                    }
                }
            }
        }
    }else{
        qDebug()<< "Critical Error. Something went completely wrong.";
    }
}

void MyThread::disconnected()
{
    int i,j;
    i=0;
    while ((i<10) && (ClientsIDs[i]!=socketDescriptor)) i++;
    if (i<10)
    {
    qDebug() << "Client" << i << " (" << socket->peerAddress().toString() << "-" << socketDescriptor << ") disconnected, not free yet";
    ClientsIDs[i]=1; //disconnected not free yet
    ClientsMask[i]=0;
    for (j=0;j<10;j++)
        {
         if (ClientsIDs[j]>1) ClientsMask[i]|=(1<<j);
        }
    ClientTimeOut[i]=timeOut1.elapsed();
    socket->deleteLater();
    exit(0);
    }
}
