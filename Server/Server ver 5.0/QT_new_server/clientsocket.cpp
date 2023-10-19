#include "clientsocket.h"

ClientSocket::ClientSocket(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::disconnected, this, &ClientSocket::disconnected);
}
