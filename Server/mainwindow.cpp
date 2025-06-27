#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QNetworkInterface>
#include<QMessageBox>
#include<QtEndian>
#include<QDateTime>
#include "fileworker.h"
#include "msgworker.h"
#include"baseworker.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<QHostAddress>addlist=QNetworkInterface::allAddresses();//获取局域网中的IPV4地址
    foreach (auto &var, addlist) {
        if( var.protocol()==QAbstractSocket::IPv4Protocol){
            if(var.toString().startsWith("192.168.")){
                ip=var.toString();
                break;
            }
        }
    }

    if(ip.isEmpty()){
        QMessageBox::warning(this,"注意","没有提取到服务器在局域网中的IPV4地址");
    }

    msgserver=new MyTcpServer(this);
    msgserver->listen(QHostAddress::Any,port);
    connect(msgserver,&MyTcpServer::newDescriptor,this,&MainWindow::do_msgnewConnection);
    fileserver=new MyTcpServer(this);
    fileserver->listen(QHostAddress::Any,port+1);
    connect(fileserver,&MyTcpServer::newDescriptor,this,&MainWindow::do_filenewConnection);

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){//通过udp广播服务器ip地址和端口号
        QByteArray data=QString("SERVER:%1:%2").arg(ip).arg(msgserver->serverPort()).toUtf8();
        QUdpSocket udpSocket;
        udpSocket.writeDatagram(data, QHostAddress::Broadcast, 45454);
    });
    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::do_msgnewConnection(qintptr socket)//处理客户端msgsocket的连接
{
    MsgWorker *worker=new MsgWorker(socket);
    worker->start();
    connect(worker,&MsgWorker::deletethread,this,[worker](){
        worker->exit();
        worker->wait();
        worker->deleteLater();
        qDebug()<<"成功删除文字线程"<<'\n';
    });
}

void MainWindow::do_filenewConnection(qintptr socket)//处理客户端filesocket的连接
{
    FileWorker *worker=new FileWorker(socket);
    worker->start();
    connect(worker,&FileWorker::deletethread,this,[worker](){
        worker->exit();
        worker->wait();
        worker->deleteLater();
        qDebug()<<"成功删除文件线程"<<'\n';
    });
}
