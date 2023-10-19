#include "server.h"
#include <QFile>

QFile file("logs.txt");



Server::Server()
{
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "start";
    }
    else {
        qDebug() << "error";
    }
    nextBlockSize = 0;

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &Server::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void Server::slotReadyRead()
{
    file.open(QFile::WriteOnly | QIODevice::Append);
    QTextStream fileStream(&file);
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);
    if (in.status() == QDataStream::Ok) {
        for (;;) {
            if (nextBlockSize == 0) {
                //qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2) {
                    //qDebug() << "Data < 2, break";
                    break;
                }
                in >> nextBlockSize;
            }
            if (socket->bytesAvailable() < nextBlockSize) {
                //qDebug() << "Data not full, break";
                break;
            }

            QString str;
            in >> str;
            nextBlockSize = 0;
            qDebug() << str;
            fileStream << str << "\n";
            break;
        }
    }
    else {
        qDebug() << "DataStream error";
    }
    file.close();
}

//void Server::SendToClient(QString str)
//{
//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_6);
//    out << str;
//    socket->write(Data);
//}
