#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void newConnection(qintptr socketDescriptor);
};

#endif // SERVER_H






