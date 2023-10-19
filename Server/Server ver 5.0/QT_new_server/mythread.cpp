#include "mythread.h"

#include <QJsonObject>
#include <QJsonDocument>

QString logStringForTXTFile;
QString lastResult = "";


MyThread::MyThread(qintptr socketDescriptor, QObject *parent) :
    QThread(parent), socketDescriptor(socketDescriptor)
{
    number = 0;
}

MyThread::~MyThread()
{
    delete socket;
}

void MyThread::run()
{
    socket = new QTcpSocket();
    socket->setSocketDescriptor(socketDescriptor);

    qDebug() << "New client: " << socketDescriptor;

    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);

    exec();

}

void MyThread::onReadyRead()
{
    qDebug() << "message from: " << socketDescriptor;

    //QByteArray data = socket->readAll();
    //QDataStream data(socket->readAll());
    //data.setVersion(QDataStream::Qt_5_6);

    //qDebug() << data;
    //QString str = QString::fromUtf8(data);
    QString str(socket->readAll());
    qDebug() <<  str;


    //qDebug() << str;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    messageHendler(str, currentDateTime);

    //socket->disconnectFromHost();
}

void MyThread::onDisconnected()
{
    socket->close();
    qDebug() << "Client " << socketDescriptor << ": Disconnected";
    quit();
}

// запись в txt
void MyThread::doSaveLog()
{
    emit resultJsonStringReady(logStringForTXTFile);
}

void MyThread::messageHendler(QString str, QDateTime currentDateTime)
{
    //QString ms = "сообщение" + str;
    QJsonObject msObject;


    QJsonDocument jsonReceivedDoc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject jsonReceivedObj = jsonReceivedDoc.object();

    // запись в txt
    MakeJsonLogString(str);
    doSaveLog();

    qDebug() << "..." << jsonReceivedObj["klick"].toString() << "...";

    if (jsonReceivedObj["calculate"].toBool()) {
        msObject = makeMessage(str, currentDateTime);
        //qDebug() << ms;
        //qDebug() << "...if..." << jsonReceivedObj["klick"].toString() << "...if...";

        if (lastResult != msObject["result"].toString()){
            lastResult = msObject["result"].toString();
            QJsonDocument msDock(msObject);
            QString jsonStr = msDock.toJson();
            socket->write(jsonStr.toUtf8());
        }
    }




}

QJsonObject MyThread::makeMessage(QString str, QDateTime currentDateTime)
{
    // Возвращаемая строка содержащая json сообщение
//    QString message;

    QJsonObject jsonObject;

    // Номер вычисления(number)
    number = number + 1;
    jsonObject["number"] = QString::number(number);

    // Время клика (time_of_click)
    QJsonDocument jsonReceivedDoc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject jsonReceivedObj = jsonReceivedDoc.object();
    jsonObject["time_of_click"] = jsonReceivedObj["time_of_click"].toString();

    // Время получения сообщения (get_time)
    QDateTime getTime = currentDateTime;
    jsonObject["get_time"] = getTime.toString(Qt::ISODate);

    // Выражение (expression)
    jsonObject["expression"] = jsonReceivedDoc["expression"];


    // Результат (result)
    //jsonObject["result"] = "результат числовой";
    jsonObject["result"] = calculateOperation(jsonReceivedObj["expression"].toString());


    // Время вычисления (calculation_duration)
    QDateTime time_of_click = QDateTime::fromString(jsonReceivedObj["time_of_click"].toString(), Qt::ISODate);
    jsonObject["calculation_duration"] = QString::number(time_of_click.msecsTo(getTime));

//    QJsonDocument jsonDock(jsonObject);
//    QString jsonStr = jsonDock.toJson();

//    message = jsonStr;

//    return message;
    return jsonObject;
}

// Метод для генерации строки логов
QString MyThread::MakeJsonLogString(QString str)
{
    //QString jsonLogString;

    QJsonObject jsonObject;

    // duration
    jsonObject["socketDescriptor"] = "socketDescriptor: " + QString::number(socketDescriptor);

    // klick
    QJsonDocument jsonReceivedDoc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject jsonReceivedObj = jsonReceivedDoc.object();
    jsonObject["click"] = jsonReceivedObj["click"].toString();

    // Время клика (time_of_click)
    jsonObject["time_of_click"] = jsonReceivedObj["time_of_click"].toString();

    // Выражение (expression)
    jsonObject["expression"] = jsonReceivedDoc["expression"];

    QJsonDocument jsonDock(jsonObject);
    QString jsonStr = jsonDock.toJson();

    //jsonLogString = jsonStr;
    logStringForTXTFile = jsonStr;

    return logStringForTXTFile;
}


