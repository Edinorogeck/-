#include "myserver.h"
#include "mythread.h"

#include <QFile>

QString fileName = "logs.txt";
QFile logFile(fileName);
//QMutex mutex;

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
    if (listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server work";
    } else {
        qDebug() << "Error server work";
    }
    //QFile logFile("logs.txt");
    //QMutex* mutex = new QMutex();

}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    MyThread* thread = new MyThread(socketDescriptor);

    //connect(&thread, &MyThread::resultJsonStringReady, [this](QString str){WriteToFile(str);});
    //connect(&thread, &MyThread::resultJsonStringReady, [](){WriteToFile();});

    connect(thread, &MyThread::resultJsonStringReady, this, &MyServer::onResultJsonStringReady);


    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void MyServer::WriteToFile(QString str)
{


    if (logFile.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&logFile);
        out << str << endl;
        logFile.close();
    } else {
        qDebug() << "error file open";
    }


}

void MyServer::onResultJsonStringReady(QString str)
{
    mutex.lock();
    //qDebug() << "onResultJsonStringReady: " << str;
    WriteToFile(str);
    mutex.unlock();
}




