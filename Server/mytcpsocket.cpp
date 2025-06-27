#include "mytcpsocket.h"

MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{

}

void MyTcpSocket::incomingConnection(qintptr socketDescriptor)
{
    emit newDescriptor(socketDescriptor);
}
