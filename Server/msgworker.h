#ifndef MSGWORKER_H
#define MSGWORKER_H

#include <QObject>
#include<QThread>
#include<QTcpSocket>
#include "baseworker.h"
#include "linksql.h"

class MsgWorker : public QThread,BaseWorker
{
    Q_OBJECT
private:
    QTcpSocket *msgsocket;
    qintptr m_sock;
    LinkSQL *sql;

public:
    explicit MsgWorker(qintptr sock,QObject *parent = nullptr);

    void disconnect()override;

    ~MsgWorker();

protected:
    void run()override;

private:
    void managejson(QByteArray data,QTcpSocket *msgtcp);
    void packingfile(const QByteArray &chunk, qint64 fileid, QString mark,int toid);
    void addfilequeue(QQueue<FileTransferContext> *queue, qint64 fileid,QString type,qint64 size,int id);
    void addfriend(QString type,QString mark,int userid,int friendid,QString friendname,bool isgroupchat,int allsize=0,int cursize=0);
    void onlinesendmsg(int id);
    void sendonlinemsg(int id,QByteArray &data);
    void sendgroupchat(QList<int> &list, QString chatname,int groupchatid);//处理新建的群聊
    void initgroupchat(int myid,int groupchatid,int groupmemberid,bool path,QString groupmembername);
    void sendgroupmark(QList<int> list, QString mark);
    void sendmsgingroup(int groupchatid, int sender, QString msg, int id);

private slots:
    void msgreaddata(QTcpSocket *msgtcp);

public slots:
    void otherthreadsendmsg(QByteArray header)override;

public:signals:
    void deletethread();
    void senddatasignal(QByteArray data)override;
};

#endif // MSGWORKER_H
