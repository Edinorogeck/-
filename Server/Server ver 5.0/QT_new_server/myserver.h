#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include <QMutex>


#include <mythread.h>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer(QObject *parent = nullptr);
    //void startServer(int port);

public:
    void incomingConnection(qintptr socketDescriptor);
    void WriteToFile(QString str);

signals:

public slots:
    void onResultJsonStringReady(QString str);
private:
    QMutex mutex;

};

#endif // MYSERVER_H
