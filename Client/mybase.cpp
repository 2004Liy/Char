#include "mybase.h"
#include<QJsonDocument>
#include <QtEndian>
#include<QFile>
#include<QJsonArray>

MyBase::MyBase(QString mark1,QObject *parent):QObject{parent},mark(mark1)
{}

MyBase::~MyBase(){}

Register1::Register1(QString mark1, QObject *parent):MyBase(mark1,parent)
{}

void Register1::manage(const QJsonObject &json,QString type)
{
    int num=json["num"].toInt();
    if(num==1){
        int id=json["id"].toInt();
        QString name=json["name"].toString();
        qint64 size=static_cast<qint64>(json["size"].toDouble());
        qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
        emit addfilequeuesignal(fileid,type,size,0,"","",0,0,0);
        emit logonsuccsee1(id,name);
        qDebug()<<"登录成功"<<'\n';
    }else if(num==0){
        emit logonfailure();
        qDebug()<<"登录失败"<<'\n';
    }else if(num==2){
        emit logoning();
        qDebug()<<"其他设备登录"<<'\n';
    }
}

void Register1::mysetid(int m_id)
{Q_UNUSED(m_id)}

Register1::~Register1()
{}

Login::Login(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Login::manage(const QJsonObject &json, QString type)
{
    int num=json["num"].toInt();
    if(num==1){
        qDebug()<<"收到注册返回"<<'\n';
        int id=json["id"].toInt();
        QString name=json["name"].toString();
        qint64 size=static_cast<qint64>(json["size"].toDouble());
        qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
        emit addfilequeuesignal(fileid,type,size,0,"","",0,0,0);
        emit loginsuccsee(id,name);
    }else{
        emit loginfailure();
    }
}

void Login::mysetid(int m_id)
{Q_UNUSED(m_id)}

Login::~Login()
{}

InitfriendOragreeadd::InitfriendOragreeadd(QString mark, QObject *parent):MyBase(mark,parent)
{}

void InitfriendOragreeadd::manage(const QJsonObject &json, QString type)
{
    int friendid=json["friendid"].toInt();
    int allnum=json["allnum"].toInt();
    int curnum=json["curnum"].toInt();
    QString friendname=json["friendname"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,friendid,friendname,"",allnum,curnum,0);
    QJsonObject filejson;
    filejson["mark"]=mark;
    QJsonDocument filedoc(filejson);
    QByteArray filedata = filedoc.toJson(QJsonDocument::Compact);
    QByteArray newfiledata;
    qint64 len = qToBigEndian<qint64>(filedata.size());
    qint64 filemark = qToBigEndian<qint64>(5);
    newfiledata.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
    newfiledata.append(reinterpret_cast<const char*>(&len), sizeof(len));
    newfiledata.append(filedata);
    emit senddata(newfiledata);
}

void InitfriendOragreeadd::mysetid(int m_id)
{Q_UNUSED(m_id)}

InitfriendOragreeadd::~InitfriendOragreeadd()
{}

Selectuser::Selectuser(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Selectuser::manage(const QJsonObject &json, QString type)
{
    int num=json["num"].toInt();
    if(num){
        int friendid=json["friendid"].toInt();
        QString friendname=json["friendname"].toString();
        qint64 size=static_cast<qint64>(json["size"].toDouble());
        qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
        emit addfilequeuesignal(fileid,type,size,friendid,friendname,"",0,0,0);
    }else{
        emit isnullid();
    }
}

void Selectuser::mysetid(int m_id)
{Q_UNUSED(m_id)}

Selectuser::~Selectuser()
{}

Friendrequest::Friendrequest(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Friendrequest::manage(const QJsonObject &json, QString type)
{
    int friendid=json["friendid"].toInt();
    QString friendname=json["friendname"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,friendid,friendname,"",0,0,0);
}

void Friendrequest::mysetid(int m_id)
{Q_UNUSED(m_id)}

Friendrequest::~Friendrequest()
{}

Sendmsg::Sendmsg(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Sendmsg::manage(const QJsonObject &json, QString type)
{
    Q_UNUSED(type)
    int fromid=json["fromid"].toInt();
    QString msg=json["msg"].toString();
    emit receivesendmsg(fromid,msg);
}

void Sendmsg::mysetid(int m_id)
{Q_UNUSED(m_id)}

Sendmsg::~Sendmsg()
{}

Sendfile::Sendfile(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Sendfile::manage(const QJsonObject &json, QString type)
{
    int fromid=json["fromid"].toInt();
    int toid=json["toid"].toInt();
    QString filename=json["filename"].toString();
    QString suffix=json["suffix"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    Q_UNUSED(fileid)
    QString path="chat/"+QString::number(fromid)+"to"+QString::number(toid)+filename;
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.close();
    emit frinedsendfile(size,filename,suffix,fromid);
    emit addfilequeuesignal(fileid,type,size,fromid,"",path,0,0,0);
}

void Sendfile::mysetid(int m_id)
{Q_UNUSED(m_id)}

Sendfile::~Sendfile()
{}

Sendpicture::Sendpicture(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Sendpicture::manage(const QJsonObject &json, QString type)
{
    int fromid=json["fromid"].toInt();
    int h=json["h"].toInt();
    int w=json["w"].toInt();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,fromid,"","",h,w,0);
}

void Sendpicture::mysetid(int m_id)
{Q_UNUSED(m_id)}

Sendpicture::~Sendpicture()
{}

Friendpicturechanged::Friendpicturechanged(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Friendpicturechanged::manage(const QJsonObject &json, QString type)
{
    qDebug()<<"收到更换头像信号"<<'\n';
    int friendid=json["friendid"].toInt();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,friendid,"","",0,0,0);
}

void Friendpicturechanged::mysetid(int m_id)
{Q_UNUSED(m_id)}

Friendpicturechanged::~Friendpicturechanged()
{}

Friendnamechanged::Friendnamechanged(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Friendnamechanged::manage(const QJsonObject &json, QString type)
{
    Q_UNUSED(type)
    int friendid=json["friendid"].toInt();
    QString friendnewname=json["friendnewname"].toString();
    emit friendnamechanged(friendid,friendnewname);
}

void Friendnamechanged::mysetid(int m_id)
{Q_UNUSED(m_id)}

Friendnamechanged::~Friendnamechanged()
{}

Creategroupchat::Creategroupchat(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Creategroupchat::manage(const QJsonObject &json, QString type)
{
    qDebug()<<"收到群聊json"<<'\n';
    int groupchatid=json["id"].toInt();
    QString groupchatname=json["name"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,groupchatid,groupchatname,"",0,0,0);
}

void Creategroupchat::mysetid(int m_id)
{Q_UNUSED(m_id)}

Creategroupchat::~Creategroupchat()
{}

Setgroupchat::Setgroupchat(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Setgroupchat::manage(const QJsonObject &json, QString type)
{
    qDebug()<<"-------------收到初始群聊标记"<<'\n';
    QJsonArray array=json["groupid"].toArray();
    QList<int>list;
    qDebug()<<"群聊id: ";
    for(int i=0;i<array.count();i++){
        list<<array[i].toInt();
        qDebug()<<array[i].toInt()<<" ";
    }
    qDebug()<<'\n';
    emit setgroupchat(list);
    qDebug()<<"群聊标记完成"<<'\n';
}

void Setgroupchat::mysetid(int m_id)
{Q_UNUSED(m_id)}

Setgroupchat::~Setgroupchat()
{}

Initgroupchat::Initgroupchat(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Initgroupchat::manage(const QJsonObject &json, QString type)
{
    int groupchatid=json["groupchatid"].toInt();
    int groupmemberid=json["groupmemberid"].toInt();
    QString groupmembername=json["groupmembername"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    addfilequeuesignal(fileid,type,size,groupchatid,groupmembername,"",groupmemberid,0,0);
}

void Initgroupchat::mysetid(int m_id)
{Q_UNUSED(m_id)}

Initgroupchat::~Initgroupchat()
{}

Groupchatmsg::Groupchatmsg(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Groupchatmsg::manage(const QJsonObject &json, QString type)
{
    int groupchatid=json["groupchatid"].toInt();
    QString msg=json["msg"].toString();
    int senderid=json["sender"].toInt();
    emit groupchatmsg(groupchatid,msg,senderid);
}

void Groupchatmsg::mysetid(int m_id)
{Q_UNUSED(m_id)}

Groupchatmsg::~Groupchatmsg()
{}

Sendpixingroup::Sendpixingroup(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Sendpixingroup::manage(const QJsonObject &json, QString type)
{
    qDebug()<<"收到群聊图片头部"<<'\n';
    int groupchatid=json["groupchatid"].toInt();
    int senderid=json["senderid"].toInt();
    int h=json["h"].toInt();
    int w=json["w"].toInt();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    emit addfilequeuesignal(fileid,type,size,senderid,"","",h,w,groupchatid);
}

void Sendpixingroup::mysetid(int m_id)
{Q_UNUSED(m_id)}

Sendpixingroup::~Sendpixingroup()
{}

Sendfiletogroup::Sendfiletogroup(QString mark, QObject *parent):MyBase(mark,parent)
{}

void Sendfiletogroup::manage(const QJsonObject &json, QString type)
{
    qDebug()<<"收到文件前缀"<<'\n';
    int senderid=json["senderid"].toInt();
    int groupid=json["groupchatid"].toInt();
    QString filename=json["filename"].toString();
    QString suffix=json["suffix"].toString();
    qint64 size=static_cast<qint64>(json["size"].toDouble());
    qint64 fileid=static_cast<qint64>(json["fileid"].toDouble());
    QString path="chat/"+QString::number(senderid)+"to"+QString::number(this->id)+filename;
    emit ingroupsendfile(senderid,size,filename,suffix,groupid);
    emit addfilequeuesignal(fileid,type,size,0,"",path,0,0,0);
}

void Sendfiletogroup::mysetid(int m_id)
{
    this->id=m_id;
}

Sendfiletogroup::~Sendfiletogroup()
{}
