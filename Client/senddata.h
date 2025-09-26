#ifndef SENDDATA_H
#define SENDDATA_H

#include <QObject>
#include<QTcpSocket>
#include<QUdpSocket>
#include<QQueue>
#include<QMutex>
#include<QJsonDocument>
#include<QJsonObject>
#include<QPixmap>
#include<FileTransferContext.h>
#include<QFile>
#include<QTimer>
#include<QMap>
#include<QList>
#include "mybase.h"

class SendData : public QObject
{
    Q_OBJECT
public:
    explicit SendData(QObject *parent = nullptr);
    ~SendData();

private:
    QUdpSocket *udpsocket=NULL;
    QTcpSocket *msgsocket=NULL;
    QTcpSocket *filesocket=NULL;
    QQueue<QByteArray>msgqueue;
    QMutex msgmutex;
    QQueue<FileTransferContext> filequeue;
    QQueue<BeforFiledata>beforfilequeue;
    QMutex beforfilemutex;
    QMutex filemutex;
    QString mark;
    int id;
    QString name;
    QPixmap profilephoto;
    QTimer *timer1;
    QMap<QString,MyBase*>*map=nullptr;
    QList<MyBase*>mylist;

private:
    void getudpsocket();
    void connectToServer(QString ip,int port);
    void packingjson(const QByteArray &m_data);
Q_INVOKABLE void handleMessages();
    void processJson(const QJsonObject &json);
void addfilequeue(qint64 fileid,QString type,qint64 size,int friendid=0,QString friendname="",QString path="",int h=0,int w=0,int groupid=0);
    BeforFiledata* selectfileid(qint64 fileid);
    void getgroupchatmark();

public:
    void init();

private slots:
    void on_msgreadyRead();
    void on_filereadyRead();


public slots:
    void on_userlogon(int id,QString password);
    void on_login(QString name,QString password);
    void on_updateinformation(int id,QString name,QString path);
    void on_updatenameonly(int id,QString name);
    void on_selectfriendid(int id);
    void on_addfriendrequest(int toid);
    void on_agreeaddfriend(int addid);
    void on_sendtofriend(int toid,QString msg,int fromid);
    void on_sendfiletofriend(int toid,QString path);
    void on_sendpicture(QString path,int toid);
    void on_closehome();
    void on_creategroup(QList<int>list,QString groupname);
    void on_groupchatsign();
    void on_groupchataddmember(int groupchatid,QList<int>list);
    void on_sendmsgtogroupchat(int groupchatid,QString msg,int id);
    void on_sendpixtogroupchat(int groupchatid,QString path,int senderid);
    void on_sendfiletogroup(int groupid,QString path,int senderid);

signals:
    void logonsuccsee(int id,QString name);
    void logonfailure();
    void logonsuccseeicon(QByteArray icon);
    void loginsuccsee(int id,QString name);
    void loginfailure();
    void loginsuccseeicon(QByteArray icon);
    void initfriend(int id,QString name,QByteArray data);
    void isnullid();
    void showselectuser(int id,QString name,QByteArray data);
    void showfriendrequest(int id,QString name,QByteArray data);
    void receivesendmsg(int fromid,QString msg);
    void mysendsize(int value);
    void friendsendfilesize(int value);
    void frinedsendfile(qint64 size,QString filename,QString suffix,int fromid);
    void friendsendpicture(int fromid,QByteArray data,int h,int w);
    void deletesocket();
    void logoning();
    void friendpicturechanged(int friendid,QByteArray data);
    void friendnamechanged(int friendid,QString friendname);
    void creategroupchat(QString name,int id,QByteArray data);
    void setgroupchat(QList<int>list);
    void addgroupmember(int groupchatid,int groupmemberid,QString groupmembername,QByteArray data);
    void groupchatmsg(int groupchatid,QString msg,int senderid);
    void sendpixingroup(int groupchatid,int senderid,QByteArray data,int h,int w);
    void ingroupsendfile(int senderid,qint64 size,QString filename,QString suffix,int groupid);
    void runinit();
};
#endif // SENDDATA_H
