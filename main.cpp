// main.cpp

#include <QCoreApplication>
#include "myserver.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

QTime timeOut1;
int authcode;
int portvalue;
int authvalue;
QString bindvalue;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption portParam(QStringList() << "p" << "port",
            "Will determine on what port the server will listen for incoming packets. (Recommended 1024-65535)",
            "portnumber",
            "0");
    parser.addOption(portParam);

    QCommandLineOption authParam(QStringList() << "a" << "auth",
            "Pick AuthCode manually, instead of randomly generating it. (Between 1 and 99999)",
            "AuthCode",
            "0");
    parser.addOption(authParam);

    QCommandLineOption bindParam(QStringList() << "b" << "bind",
            "Address to bind to",
            "bindaddress",
            "0");
    parser.addOption(bindParam);

    // Process the actual command line arguments given by the user
    parser.process(a);

    QString portstring = parser.value(portParam);
    portvalue = portstring.toInt();
    QString authstring = parser.value(authParam);
    authvalue = authstring.toInt();
    QString bindstring = parser.value(bindParam);
    bindvalue = bindstring;

    // Make a server and starts it
    MyServer server;
    server.startServer();

    return a.exec();

}
