// mythread.h

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QTime>





class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID, QObject *parent = 0);

    void run();
    QString authSuccess;
    char writeAuth[128];
    char writeAdmin[128];
    QByteArray tempAuth1;
    const char* tempAuth2;

signals:
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    char tmp1[64];
    int connectTries = 0;
};

#endif // MYTHREAD_H
