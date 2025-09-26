#ifndef MYBASE_H
#define MYBASE_H
#include<QObject>
#include<QJsonObject>
#include<QList>
class MyBase:public QObject
{
Q_OBJECT
public:
    MyBase(QString mark,QObject *parent = nullptr);
    virtual void manage(const QJsonObject &json,QString type)=0;
    virtual void mysetid(int m_id)=0;
    virtual ~MyBase()=0;

protected:
    QString mark;

signals:
    void addfilequeuesignal(qint64 fileid,QString type,qint64 size,int friendid,QString friendname,QString path,int h,int w,int groupid);

};

class Register1:public MyBase{
Q_OBJECT
public:
    Register1(QString mark,QObject *parent = nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Register1()override;

signals:
    void logonsuccsee1(int id,QString name);
    void logonfailure();
    void logoning();
};

class Login:public MyBase{
Q_OBJECT
public:
    Login(QString mark,QObject *parent = nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Login()override;

signals:
    void loginsuccsee(int id,QString name);
    void loginfailure();
};

class InitfriendOragreeadd:public MyBase{
Q_OBJECT
public:
    InitfriendOragreeadd(QString mark,QObject *parent = nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~InitfriendOragreeadd()override;

signals:
    void senddata(QByteArray &byte);
};

class Selectuser:public MyBase{
Q_OBJECT
public:
    Selectuser(QString mark,QObject *parent = nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Selectuser();

signals:
    void isnullid();
};

class Friendrequest:public MyBase{
Q_OBJECT
public:
    Friendrequest(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Friendrequest();
};

class Sendmsg:public MyBase{
Q_OBJECT
public:
    Sendmsg(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Sendmsg();

signals:
    void receivesendmsg(int id,QString msg);
};

class Sendfile:public MyBase{
Q_OBJECT
public:
    Sendfile(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Sendfile();

signals:
    void frinedsendfile(qint64 size,QString friendname,QString suffix,int fromid);
};

class Sendpicture:public MyBase{
Q_OBJECT
public:
    Sendpicture(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Sendpicture();
};

class Friendpicturechanged:public MyBase{
Q_OBJECT
public:
    Friendpicturechanged(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Friendpicturechanged();
};

class Friendnamechanged:public MyBase{
Q_OBJECT
public:
    Friendnamechanged(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Friendnamechanged();

signals:
    void friendnamechanged(int id,QString name);
};

class Creategroupchat:public MyBase{
Q_OBJECT
public:
    Creategroupchat(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Creategroupchat();
};

class Setgroupchat:public MyBase{
Q_OBJECT
public:
    Setgroupchat(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Setgroupchat();

signals:
    void setgroupchat(QList<int> list);
};

class Initgroupchat:public MyBase{
Q_OBJECT
public:
    Initgroupchat(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Initgroupchat();
};

class Groupchatmsg:public MyBase{
Q_OBJECT
public:
    Groupchatmsg(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Groupchatmsg();

signals:
    void groupchatmsg(int groupchatid,QString msg,int senderid);
};

class Sendpixingroup:public MyBase{
Q_OBJECT
public:
    Sendpixingroup(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Sendpixingroup();
};

class Sendfiletogroup:public MyBase{
Q_OBJECT
public:
    Sendfiletogroup(QString mark,QObject *parent=nullptr);
    void manage(const QJsonObject &json,QString type)override;
    void mysetid(int m_id)override;
    ~Sendfiletogroup();

private:
    int id;

signals:
    void ingroupsendfile(int senderid,qint64 size,QString filename,QString suffix,int groupid);
};

#endif // MYBASE_H
