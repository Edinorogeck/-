#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H


#include <QObject>
#include "server.h"

class ServerManager : public QObject
{
    Q_OBJECT

public:
    ServerManager(QObject *parent = nullptr);

public slots:
    void startServer();
    void handleNewConnection(qintptr socketDescriptor);

private:
    Server *server;
};




#endif // SERVERMANAGER_H
