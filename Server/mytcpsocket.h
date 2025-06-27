#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include<QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

signals:
    void newDescriptor(qintptr socket);
};

#endif // MYTCPSOCKET_H
