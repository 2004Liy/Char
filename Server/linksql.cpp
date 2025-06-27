#include "linksql.h"
#include<QMessageBox>
QThreadStorage<QSqlDatabase*> threadDBStorage;
LinkSQL::LinkSQL(QObject *parent)
    : QObject{parent}
{
    sql = createConnection();
    if(!sql.open()){
        QMessageBox::warning(NULL,"注意","数据库连接错误");
        return;
    }else{
        qDebug()<<"打开成功"<<'\n';
    }
}


QSqlDatabase LinkSQL::createConnection()
{
    QString connName = QString("conn_%1").arg(quintptr(QThread::currentThreadId()));//用线程地址生成唯一的连接id
    if (!QSqlDatabase::contains(connName)) {
        QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", connName));
        db->setHostName("localhost");
        db->setUserName("root");
        db->setPassword("20041203Li?");
        db->setDatabaseName("test");
        if (!db->open()) {
            qCritical() << "Failed to open database:" << db->lastError();
            return QSqlDatabase();
        }else{
            threadDBStorage.setLocalData(db);
        }
    }
    return QSqlDatabase::database(connName);
}

bool LinkSQL::selectuser(int id, QString password)
{
    QSqlQuery q(sql);
    q.prepare("select password from users where id=?;");
    q.addBindValue(id);
    q.exec();
    if(q.next()){
        QString spassword=q.value(0).toString();
        if(spassword==password){
            return 1;
        }
    }
    return 0;
}



QString LinkSQL::selecticon(int id)
{
    QSqlQuery q(sql);
    q.prepare("select path from users where id=?;");
    q.addBindValue(id);
    q.exec();
    if(q.next()){
        return q.value(0).toString();
    }
    return "";
}

int LinkSQL::insertuser(QString name, QString password)
{
    QSqlQuery q(sql);
    q.prepare("insert into users (username, password) values (?, ?)");
    q.addBindValue(name);
    q.addBindValue(password);

    if (!q.exec()) {
        qCritical() << "Insert failed:" << q.lastError();
        return 0;
    }
    QVariant idVar = q.lastInsertId();
    int newId = idVar.toInt();
    return newId;
}

void LinkSQL::updateinformation(int id, QString name, QString path)
{
    QSqlQuery q(sql);
    q.prepare("update users set username=?,path=? where id=?");
    q.addBindValue(name);
    q.addBindValue(path);
    q.addBindValue(id);
    q.exec();
}

void LinkSQL::updatenameonly(int id, QString name)
{
    QSqlQuery q(sql);
    q.prepare("update users set username=? where id=?");
    q.addBindValue(name);
    q.addBindValue(id);
    q.exec();
}



QString LinkSQL::selectname(int id)
{
    QSqlQuery q(sql);
    q.prepare("select username from users where id=?;");
    q.addBindValue(id);
    q.exec();
    if(q.next()){
        return q.value(0).toString();
    }
    return "";
}



void LinkSQL::selectfriend(int id)
{
    QSqlQuery q(sql);
    q.prepare("select friendid from friendships where userid=?");
    q.addBindValue(id);
    q.exec();
    int total=q.size();
    int cur=0;
    while(q.next()){
        cur++;
        int friendid=q.value(0).toInt();
        qDebug()<<"id:1"<<friendid<<'\n';
        emit frienddata(friendid);
        if(cur==total){
            emit friendok();
        }
    }
}



void LinkSQL::addfriend(int userid, int friendid)
{
    QSqlQuery q(sql);
    q.prepare("insert into friendships(userid,friendid) value (?,?);");
    q.addBindValue(userid);
    q.addBindValue(friendid);
    q.exec();
    q.prepare("insert into friendships(userid,friendid) value (?,?);");
    q.addBindValue(friendid);
    q.addBindValue(userid);
    q.exec();
}

void LinkSQL::insertgroupchat(int id)
{
    QSqlQuery q(sql);
    q.prepare("insert into groupchat(id) value (?);");
    q.addBindValue(id);
    q.exec();
    QString tablename="";
    int p=id;
    while(p){
        char a='a'+(p%10);
        tablename+=a;
        p/=10;
    }
    q.prepare(QString("CREATE TABLE IF NOT EXISTS %1("
                      "id int not null"
                      ");").arg(tablename));
    q.exec();
    emit createtableok();
}

void LinkSQL::addmember(QList<int> list,int id)
{
    QSqlQuery q(sql);
    QString tablename="";
    int p=id;
    while(p){
        char a='a'+(p%10);
        tablename+=a;
        p/=10;
    }
    for(int i=0;i<list.count();i++){
        q.prepare("insert into friendships(userid,friendid) value (?,?);");
        q.addBindValue(list[i]);
        q.addBindValue(id);
        q.exec();
        q.prepare(QString("insert into %1(id) value (?);").arg(tablename));
        q.addBindValue(list[i]);
        q.exec();
    }
}

bool LinkSQL::isgroupchat(int id)
{
    QSqlQuery q(sql);
    q.prepare("select *from groupchat;");
    q.exec();
    while(q.next()){
        int groupchatid=q.value(0).toInt();
        if(groupchatid==id){
            return 1;
        }
    }
    return 0;
}

void LinkSQL::selectgroupmembers(int groupchatid)
{
    QString s="";
    int p=groupchatid;
    while(p){
        char a='a'+(p%10);
        s+=a;
        p=p/10;
    }
    QSqlQuery q(sql);
    q.prepare(QString("select *from %1").arg(s));
    q.exec();
    while(q.next()){
        int id=q.value(0).toInt();
        emit groupmembersid(groupchatid,id);
        // QString pp=selecticon(id);
        // QString name=selectname(id);
        // if(pp.isEmpty()){
        //     emit groupmembersid(groupchatid,id,0,name);
        // }else{
        //     emit groupmembersid(groupchatid,id,1,name);
        // }
    }
}

int LinkSQL::friendcount(int id)
{
    QSqlQuery q(sql);
    q.prepare("select friendid from friendships where userid=?");
    q.addBindValue(id);
    q.exec();
    return q.size();
}

