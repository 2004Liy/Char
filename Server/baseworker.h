#ifndef BASEWORKER_H
#define BASEWORKER_H

#include<QObject>
#include<QMap>
#include<QMutex>
#include<QTcpSocket>
#include<QQueue>
#include<FileTransferContext.h>
#include<QMetaObject>
class BaseWorker;
struct A{
    BaseWorker *msgworker=NULL;
    BaseWorker *fileworker=NULL;
};
struct B{
    QMutex *mutex=NULL;;
    QQueue<QByteArray>* queue=NULL;

    B(){
        queue=new QQueue<QByteArray>;
        mutex=new QMutex;
    }
    ~B(){
        delete mutex;
        mutex=NULL;
        delete queue;
        queue=NULL;
    }
};
class BaseWorker
{
private:
    //QTcpSocket *m_socket=NULL;
public:
    static QMap<QString,A>socketmap;
    static QMap<int,QQueue<FileTransferContext>*>queuemap;
    static QMap<int,QMutex*>mutexmap;
    static QMap<int,QString>idmarkmap;
    static QMutex mutexmutex;
    static QMutex idmarkmutex;
    static QMutex socketmapmutex;
    static QMutex queuemapmutex;
    static qint64 m_fileid;
    static QMutex fileidmutex;
    static QMap<int,B*>offlinemsgmap;
    static QMap<int,bool>vismap;
    static QMutex vismapmutex;
    static QMutex offlinemsgmapmutex;
public:
    explicit BaseWorker();

    virtual void disconnect()=0;


public slots:
    virtual void otherthreadsendmsg(QByteArray header)=0;
signals:
    void writeFileSocketRequest(QByteArray data, QString mark);

signals:
    virtual void senddatasignal(QByteArray data)=0;
};

#endif // BASEWORKER_H
