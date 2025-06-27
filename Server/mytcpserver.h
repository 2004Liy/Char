#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include<QTcpServer>

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor);

signals:
    void newDescriptor(qintptr socket);
};

#endif // MYTCPSERVER_H
