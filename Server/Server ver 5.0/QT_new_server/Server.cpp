#include "server.h"
#include "clientsocket.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientSocket *clientSocket = new ClientSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &ClientSocket::disconnected, clientSocket, &ClientSocket::deleteLater);
    emit newConnection(socketDescriptor);
}
