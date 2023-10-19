#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <QString>
#include <QDateTime>

#include <calculateOperation.h>

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(qintptr descriptor, QObject *parent = nullptr);
    ~MyThread();

    void run();
    // запись в txt
    void doSaveLog();

public slots:
    void onReadyRead();
    void onDisconnected();




signals:
    // запись в txt
    void resultJsonStringReady(QString str);


private:
    qintptr socketDescriptor;
    QTcpSocket* socket;
    quint16 number;



    void messageHendler(QString str, QDateTime currentDateTime);
    QJsonObject makeMessage(QString str, QDateTime currentDateTime);

    QString MakeJsonLogString(QString str);

};

#endif // MYTHREAD_H
