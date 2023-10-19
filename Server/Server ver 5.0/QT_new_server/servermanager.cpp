#include "servermanager.h"

ServerManager::ServerManager(QObject *parent)
    : QObject(parent)
{
    server = new Server(this);
    connect(server, &Server::newConnection, this, &ServerManager::handleNewConnection);
}

void ServerManager::startServer()
{
    if (server->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "Server listening on port 2323";
    }
    else
    {
        qDebug() << "Unable to start server";
    }
}

void ServerManager::handleNewConnection(qintptr socketDescriptor)
{
