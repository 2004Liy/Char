#include "msgworker.h"
#include<QNetworkInterface>
#include <QtEndian>
#include "fileworker.h"

MsgWorker::MsgWorker(qintptr sock,QObject *parent)
    : QThread{parent},m_sock(sock)
{
}

void MsgWorker::otherthreadsendmsg(QByteArray data)
{
    qDebug()<<"发送数据"<<'\n';
    msgsocket->write(data);
    msgsocket->flush();
}

void MsgWorker::disconnect()
{
    msgsocket->disconnectFromHost();
}

MsgWorker::~MsgWorker()
{
    if(msgsocket){
        msgsocket->deleteLater();
    }
}

void MsgWorker::run()
{
    //sql=new LinkSQL(this);
    msgsocket=new QTcpSocket;
    msgsocket->setSocketDescriptor(m_sock);
    connect(this,&MsgWorker::senddatasignal,this,&MsgWorker::otherthreadsendmsg,Qt::DirectConnection);
    connect(msgsocket,&QTcpSocket::readyRead,this,[this](){//有数据要接受
        qDebug()<<"--------------文字数据到达"<<'\n';
        msgreaddata(msgsocket);
    },Qt::DirectConnection);
    connect(msgsocket, &QTcpSocket::disconnected,this, [this](){
        qDebug()<<"文字线程结束222222222222222"<<'\n';
        quit();
        this->wait();
        emit deletethread();

    },Qt::DirectConnection);
    exec();
}

void MsgWorker::managejson(QByteArray data, QTcpSocket *msgtcp)
{
    QJsonDocument doc=QJsonDocument::fromJson(data);
    QJsonObject request = doc.object();
    QString type = request["type"].toString();
    if(type=="connect"){
        QString mark=request["mark"].toString();
        QMutexLocker locker(&socketmapmutex);
        socketmap[mark].msgworker=this;
        qDebug()<<mark<<"文字套接字绑定完成1111111111111111"<<"\n";
    }else if(type=="logon"){//登录
        QString mark=request["mark"].toString();
        int userid=request["id"].toInt();
        QString userpassword=request["password"].toString();
        int cur=0,all=0;
        LinkSQL sql;
        all=sql.friendcount(userid);
        QList<int>list;
        connect(&sql,&LinkSQL::frienddata,this,[&](int friendid){
             cur++;
            LinkSQL sql1;
            QString friendname=sql1.selectname(friendid);
            qDebug()<<"name:1"<<friendname<<'\n';
            if(sql1.isgroupchat(friendid)){
                list<<friendid;
                addfriend("initfriend",mark,userid,friendid,friendname,1,all,cur);
            }else{
                addfriend("initfriend",mark,userid,friendid,friendname,0,all,cur);
            }
        });
        QJsonObject json;
        json["type"]="logon";
        queuemapmutex.lock();
        if(queuemap[userid]!=NULL){
            json["num"]=2;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);
            qDebug()<<"已登陆"<<'\n';
            queuemapmutex.unlock();
            return;
        }
        queuemapmutex.unlock();
        if(sql.selectuser(userid,userpassword)){
            json["num"]=1;
            json["id"]=userid;
            QString username=sql.selectname(userid);
            json["name"]=username;
            QString path=sql.selecticon(userid);
            qint64 filesize;
            QString iconpath="";
            if(path==""){
                iconpath="usericon/1.png";
            }else{
                iconpath="usericon/"+QString::number(userid)+".png";
            }
            QFile file(iconpath);
            file.open(QIODevice::ReadOnly);
            filesize=file.size();
            fileidmutex.lock();
            qint64 fileId = m_fileid++;
            if(m_fileid>1e15){
                m_fileid=0;
            }
            json["fileid"]=fileId;
            fileidmutex.unlock();
            json["size"]=filesize;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);

            const qint64 CHUNK_SIZE = 1024 * 50;
            while (!file.atEnd()) {
                QByteArray chunk = file.read(CHUNK_SIZE);
                packingfile(chunk,fileId,mark,userid);
            }
            file.close();
            queuemapmutex.lock();
            QQueue<FileTransferContext> *newqueue=new QQueue<FileTransferContext>;
            queuemap[userid]=newqueue;
            queuemapmutex.unlock();
            idmarkmutex.lock();
            idmarkmap[userid]=mark;
            idmarkmutex.unlock();
            QMutex *newmutex=new QMutex;
            mutexmutex.lock();
            mutexmap[userid]=newmutex;
            mutexmutex.unlock();
            sql.selectfriend(userid);
            vismapmutex.lock();
            if(vismap[userid]){
                QTimer::singleShot(300, [this,userid]() {
                    onlinesendmsg(userid);
                });
            }else{
                qDebug()<<"没有离线消息"<<'\n';
            }
            vismapmutex.unlock();
        }else{
            json["num"]=0;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);
        }
    }else if(type=="getgroupchatmark"){
        int id=request["id"].toInt();
        int cur=0,all=0;
        LinkSQL *sql=new LinkSQL(this);
        all=sql->friendcount(id);
        QList<int>list;
        connect(sql,&LinkSQL::frienddata,this,[&](int friendid){
            cur++;
            LinkSQL sql1;
            if(sql1.isgroupchat(friendid)){
                list<<friendid;
            }
            if(cur==all){
                QJsonObject json2;
                json2["type"]="setgroupchat";
                QJsonArray array;
                for(int i=0;i<list.count();i++){
                    array.append(list[i]);
                }
                json2["groupid"]=array;
                QJsonDocument doc(json2);
                QByteArray m_data;
                m_data = doc.toJson(QJsonDocument::Compact);
                qint64 len = qToBigEndian<qint64>(m_data.size());
                qDebug()<<"len:"<<len<<'\n';
                QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
                newdata.append(m_data);
                //emit this->senddatasignal(m_data);
                qDebug()<<"发送群聊id"<<'\n';
                this->otherthreadsendmsg(newdata);
            }
        });
        sql->selectfriend(id);
    }else if(type=="getmember"){
        qDebug()<<"收到请求成员"<<'\n';
        int id=request["id"].toInt();
        QString mark=request["mark"].toString();
        LinkSQL *sql=new LinkSQL(this);
        connect(sql,&LinkSQL::groupmembersid,[=](int groupchatid,int groupmemberid){
            qDebug()<<"获取群聊成员"<<'\n';
            LinkSQL sql1;
            bool path=1;
            if(sql1.selecticon(groupmemberid).isEmpty()){
                path=0;
            }
            QString groupmembername=sql1.selectname(groupmemberid);
            qDebug()<<"成员:"<<groupmembername<<'\n';
            initgroupchat(id,groupchatid,groupmemberid,path,groupmembername);
        });
        QList<int>list;
        connect(sql,&LinkSQL::frienddata,this,[=](int friendid){
            qDebug()<<"筛查群聊"<<'\n';
            LinkSQL sql1;
            if(sql1.isgroupchat(friendid)){
                //list<<friendid;
                qDebug()<<"群聊："<<friendid<<'\n';
                sql->selectgroupmembers(friendid);
            }
        });
        sql->selectfriend(id);
    }else if(type=="addnewmember"){
        int groupchatid=request["groupchatid"].toInt();
        QList<int>list;
        QJsonArray array=request["newmember"].toArray();
        for(int i=0;i<array.count();i++){
            list<<array[i].toInt();
        }
        QList<int>list1;
        LinkSQL sql1,sql;
        connect(&sql1,&LinkSQL::groupmembersid,[&](int groupchatid,int groupmemberid){
            Q_UNUSED(groupchatid);
            list1<<groupmemberid;
        });
        sql1.selectgroupmembers(groupchatid);
        connect(&sql,&LinkSQL::groupmembersid,[=](int groupchatid,int groupmemberid){
            LinkSQL sql2;
            bool path=1;
            if(sql2.selecticon(groupmemberid).isEmpty()){
                path=0;
            }
            QString groupmembername=sql2.selectname(groupmemberid);
            for(int i=0;i<list.count();i++){
                initgroupchat(list[i],groupchatid,groupmemberid,path,groupmembername);
            }
        });
        sql.addmember(list,groupchatid);
        QString name=sql.selectname(groupchatid);
        sendgroupchat(list,name,groupchatid);
        for(int i=0;i<list1.count();i++){
            for(int j=0;j<list.count();j++){
                bool p=0;
                if(sql.selecticon(list[j]).isEmpty()){
                    p=0;
                }else{
                    p=1;
                }
                initgroupchat(list1[i],groupchatid,list[j],p,sql.selectname(list[j]));
            }
        }
        sql.selectgroupmembers(groupchatid);
        for(int i=0;i<list.count();i++){
            idmarkmutex.lock();
            QString s=idmarkmap[list[i]];
            idmarkmutex.unlock();
            QList<int>list_;
            list_<<groupchatid;
            QTimer::singleShot(200, this, [=]() {
                sendgroupmark(list_, s);
            });
            qDebug()<<list[i]<<"初始化"<<groupchatid<<"群聊"<<'\n';
        }
    }else if(type=="login"){
        QString mark=request["mark"].toString();
        QString name=request["name"].toString();
        QString password=request["password"].toString();
        LinkSQL sql;
        int id=sql.insertuser(name,password);
        QJsonObject json;
        json["type"]="login";
        if(id){
            json["num"]=1;
            json["id"]=id;
            json["name"]=name;
            qint64 filesize;
            QFile file("usericon/1.png");
            file.open(QIODevice::ReadOnly);
            filesize=file.size();
            fileidmutex.lock();
            qint64 fileId = m_fileid++;
            if(m_fileid>1e15){
                m_fileid=0;
            }
            json["fileid"]=fileId;
            fileidmutex.unlock();
            json["size"]=filesize;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);

            const qint64 CHUNK_SIZE = 1024 * 50;
            while (!file.atEnd()) {
                QByteArray chunk = file.read(CHUNK_SIZE);
                packingfile(chunk,fileId,mark,id);
            }
            file.close();
            queuemapmutex.lock();
            QQueue<FileTransferContext> *newqueue=new QQueue<FileTransferContext>;
            queuemap[id]=newqueue;
            queuemapmutex.unlock();
            idmarkmutex.lock();
            idmarkmap[id]=mark;
            idmarkmutex.unlock();
            QMutex *newmutex=new QMutex;
            mutexmutex.lock();
            mutexmap[id]=newmutex;
            mutexmutex.unlock();
        }else{
            json["num"]=0;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);
        }
    }else if(type=="updateinformation"){
        QString name=request["name"].toString();
        int id=request["id"].toInt();
        QString path="usericon/"+QString::number(id)+".png";
        LinkSQL sql;
        connect(&sql,&LinkSQL::frienddata,this,[=](int friendid){
            QJsonObject json;
            json["type"]="friendnamechanged";
            json["friendid"]=id;
            json["friendnewname"]=name;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            socketmapmutex.lock();
            idmarkmutex.lock();
            if(socketmap[idmarkmap[friendid]].msgworker!=NULL){
                socketmap[idmarkmap[friendid]].msgworker->otherthreadsendmsg(newdata);
            }
            idmarkmutex.unlock();
            socketmapmutex.unlock();
        });
        sql.selectfriend(id);
        sql.updateinformation(id,name,path);
        qint64 size=static_cast<qint64>(request["size"].toDouble());
        qint64 fileid=static_cast<qint64>(request["fileid"].toDouble());
        queuemapmutex.lock();
        QQueue<FileTransferContext>*queue=queuemap[id];
        queuemapmutex.unlock();
        addfilequeue(queue,fileid,type,size,id);
    }else if(type=="updatenameonly"){
        QString name=request["name"].toString();
        int id=request["id"].toInt();
        LinkSQL sql;
        connect(&sql,&LinkSQL::frienddata,this,[=](int friendid){
            QJsonObject json;
            json["type"]="friendnamechanged";
            json["friendid"]=id;
            json["friendnewname"]=name;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            socketmapmutex.lock();
            idmarkmutex.lock();
            if(socketmap[idmarkmap[friendid]].msgworker!=NULL){
                socketmap[idmarkmap[friendid]].msgworker->otherthreadsendmsg(newdata);
            }
            //emit socketmap[idmarkmap[friendid]].msgworker->senddatasignal(newdata);
            idmarkmutex.unlock();
            socketmapmutex.unlock();
        });
        sql.updatenameonly(id,name);
        sql.selectfriend(id);
    }else if(type=="selectuser"){
        QString mark=request["mark"].toString();
        int id=request["id"].toInt();
        int selectid=request["selectid"].toInt();
        LinkSQL sql;
        QString selectname=sql.selectname(selectid);
        if(selectname.isEmpty()){
            QJsonObject json;
            json["type"]="selectuser";
            json["num"]=0;
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            //emit this->senddatasignal(m_data);
            this->otherthreadsendmsg(newdata);
        }else{
            addfriend(type,mark,id,selectid,selectname,0);
        }
    }else if(type=="friendrequest"){
        int fromid=request["fromid"].toInt();
        QString fromname=request["fromname"].toString();
        int toid=request["toid"].toInt();
        QString mark=idmarkmap[toid];
        addfriend(type,mark,toid,fromid,fromname,0);
    }else if(type=="agreeadd"){
        int fromid=request["fromid"].toInt();
        QString fromname=request["fromname"].toString();
        int toid=request["toid"].toInt();
        QString mark=idmarkmap[toid];
        LinkSQL sql;
        sql.addfriend(fromid,toid);
        addfriend(type,mark,toid,fromid,fromname,0);
    }else if(type=="sendmsg"){
        int fromid=request["fromid"].toInt();
        QString msg=request["msg"].toString();
        int toid=request["toid"].toInt();
        LinkSQL sql;
        if(sql.isgroupchat(toid)){

        }
        idmarkmutex.lock();
        QString mark=idmarkmap[toid];
        idmarkmutex.unlock();
        QJsonObject json;
        json["type"]=type;
        json["fromid"]=fromid;
        json["msg"]=msg;
        socketmapmutex.lock();
        if(socketmap[mark].msgworker==NULL){//如果用户没有登录把消息添加到记录队列中
            socketmapmutex.unlock();
            offlinemsgmapmutex.lock();
            vismapmutex.lock();
            if(vismap[toid]==0){
                B *b=new B;
                offlinemsgmap.insert(toid,b);
            }
            vismap[toid]=1;
            vismapmutex.unlock();
            offlinemsgmap[toid]->mutex->lock();
            QByteArray newdata;
            int p=1;
            newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
            newdata.append(data);
            offlinemsgmap[toid]->queue->enqueue(newdata);
            offlinemsgmap[toid]->mutex->unlock();
            offlinemsgmapmutex.unlock();
        }else{
            QJsonDocument doc(json);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            socketmap[mark].msgworker->otherthreadsendmsg(newdata);
            //emit socketmap[mark].msgworker->senddatasignal(newdata);
            socketmapmutex.unlock();
        }
    }else if(type=="sendfile"||type=="sendpicture"){
        int toid=request["toid"].toInt();
        idmarkmutex.lock();
        QString mark=idmarkmap[toid];
        idmarkmutex.unlock();
        socketmapmutex.lock();
        if(socketmap[mark].msgworker!=NULL){
            QJsonDocument doc(request);
            QByteArray m_data;
            m_data = doc.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            socketmap[mark].msgworker->otherthreadsendmsg(newdata);
            //emit socketmap[mark].msgworker->senddatasignal(newdata);
            socketmapmutex.unlock();
        }else{
            socketmapmutex.unlock();
            offlinemsgmapmutex.lock();
            vismapmutex.lock();
            if(vismap[toid]==0){
                B *b=new B;
                offlinemsgmap.insert(toid,b);
            }
            vismap[toid]=1;
            vismapmutex.unlock();
            offlinemsgmap[toid]->mutex->lock();
            QByteArray newdata;
            int p=1;
            newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
            newdata.append(data);
            offlinemsgmap[toid]->queue->enqueue(newdata);
            offlinemsgmap[toid]->mutex->unlock();
            offlinemsgmapmutex.unlock();
        }
    }else if(type=="creategroup"){
        qDebug()<<"接收到创建群聊信号-----------------"<<'\n';
        QString *name=new QString;
        *name=request["name"].toString();
        QList<int>*list=new QList<int>;
        QJsonArray array=request["memberid"].toArray();
        for(int i=0;i<array.count();i++){
            *list<<array[i].toInt();
            qDebug()<<"群聊成员id："<<array[i].toInt()<<'\n';
        }
        int *p=new int;
        LinkSQL sql;
        connect(&sql,&LinkSQL::createtableok,this,[&](){
            LinkSQL sql1;
            sql1.addmember(*list,*p);
            sendgroupchat(*list,*name,*p);
        });
        int id=sql.insertuser(*name,"1111");
        *p=id;
        connect(this,&MsgWorker::deletegroupdata,this,[&](){
            if(p!=NULL){
                delete p;
                p=NULL;
            }
            if(name!=NULL){
                delete name;
                name=NULL;
            }
            if(list!=NULL){
                delete list;
                list=NULL;
            }
        });
        sql.insertgroupchat(id);
    }else if(type=="sendmsgingroup"){
        int groupchatid=request["groupchatid"].toInt();
        QString msg=request["msg"].toString();
        int senderid=request["sender"].toInt();
        LinkSQL sql;
        connect(&sql,&LinkSQL::groupmembersid,[=](int groupchatid,int id){
            if(id!=senderid){
                sendmsgingroup(groupchatid,senderid,msg,id);
            }
        });
        sql.selectgroupmembers(groupchatid);
    }else if(type=="sendpixingroup"||type=="sendfiletogroup"){
        int groupchatid=request["groupchatid"].toInt();
        int senderid=request["senderid"].toInt();
        LinkSQL sql;
        connect(&sql,&LinkSQL::groupmembersid,[=](int groupchat,int id){
            Q_UNUSED(groupchat);
            if(id!=senderid){
                idmarkmutex.lock();
                QString mark=idmarkmap[id];
                idmarkmutex.unlock();
                socketmapmutex.lock();
                //QTcpSocket *msgtosocket=socketmap[mark].msgsocket;
                if(socketmap[mark].msgworker!=NULL){
                    QJsonDocument doc(request);
                    QByteArray m_data;
                    m_data = doc.toJson(QJsonDocument::Compact);
                    qint64 len = qToBigEndian<qint64>(m_data.size());
                    QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
                    newdata.append(m_data);
                    socketmap[mark].msgworker->otherthreadsendmsg(newdata);
                    //emit socketmap[mark].msgworker->senddatasignal(newdata);
                    socketmapmutex.unlock();
                }else{
                    socketmapmutex.unlock();
                    offlinemsgmapmutex.lock();
                    vismapmutex.lock();
                    if(vismap[id]==0){
                        B *b=new B;
                        offlinemsgmap.insert(id,b);
                    }
                    vismap[id]=1;
                    vismapmutex.unlock();
                    offlinemsgmap[id]->mutex->lock();
                    QByteArray newdata;
                    int p=1;
                    newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
                    newdata.append(data);
                    offlinemsgmap[id]->queue->enqueue(newdata);
                    offlinemsgmap[id]->mutex->unlock();
                    offlinemsgmapmutex.unlock();
                }
            }
        });
        sql.selectgroupmembers(groupchatid);
    }else if(type=="close"){
        qDebug()<<"清理内存"<<'\n';
        QString mark=request["mark"].toString();
        int id=request["id"].toInt();
        socketmapmutex.lock();
        socketmap[mark].msgworker->disconnect();
        socketmap[mark].fileworker->disconnect();
        socketmap.remove(mark);
        socketmapmutex.unlock();
        queuemapmutex.lock();
        delete queuemap[id];
        queuemap[id]=NULL;
        queuemap.remove(id);
        queuemapmutex.unlock();
        idmarkmutex.lock();
        idmarkmap.remove(id);
        idmarkmutex.unlock();
        mutexmutex.lock();
        mutexmap.remove(id);
        mutexmutex.unlock();
    }
}

void MsgWorker::packingfile(const QByteArray &chunk, qint64 fileid, QString mark, int toid)
{
    QByteArray header;
    qint64 m_fileid=qToBigEndian<qint64>(fileid);
    qint64 len=qToBigEndian<qint64>(chunk.size());
    header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
    header.append(reinterpret_cast<const char*>(&len), sizeof(len));
    header.append(chunk);
    socketmapmutex.lock();
    if(socketmap[mark].fileworker!=NULL){
        // BaseWorker *worker=socketmap[mark].fileworker;
        // QMetaObject::invokeMethod(worker, "otherthreadsendmsg",
        //                           Qt::QueuedConnection,
        //                           Q_ARG(QByteArray, std::move(header)));
        //emit socketmap[mark].fileworker->senddatasignal(header);
        socketmap[mark].fileworker->otherthreadsendmsg(header);
    }else{
        qDebug()<<"文件加入队列"<<'\n';
        vismapmutex.lock();
        if(!vismap[toid]){
            B *b=new B;
            offlinemsgmap.insert(toid,b);
        }
        vismap[toid]=1;
        vismapmutex.unlock();
        offlinemsgmapmutex.lock();
        offlinemsgmap[toid]->mutex->lock();
        if(offlinemsgmap[toid]->queue==NULL){
            QQueue<QByteArray>* newqueue=new QQueue<QByteArray>;
            offlinemsgmap[toid]->queue=newqueue;
        }
        QByteArray newdata;
        int p=2;
        newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
        newdata.append(header);
        offlinemsgmap[toid]->queue->enqueue(newdata);
        offlinemsgmap[toid]->mutex->unlock();
        offlinemsgmapmutex.unlock();
    }
    socketmapmutex.unlock();
}

void MsgWorker::addfilequeue(QQueue<FileTransferContext> *queue, qint64 fileid, QString type, qint64 size, int id)
{
    LinkSQL sql;
    FileTransferContext *ctx=NULL;
    for(auto &them:*queue){
        if(them.fileId==fileid){
            ctx=&them;
            break;
        }
    }
    mutexmutex.lock();
    QMutex *usermutex=mutexmap[id];
    mutexmutex.unlock();
    usermutex->lock();
    if(ctx==NULL){
        FileTransferContext newctx;
        newctx.type=type;
        newctx.fileId=fileid;
        newctx.totalSize=size;
        newctx.receivedSize=0;
        queue->enqueue(newctx);
    }else{
        ctx->totalSize=size;
        ctx->type=type;
        if(ctx->totalSize>0&&ctx->receivedSize>=ctx->totalSize){
            auto array = std::make_shared<QByteArray>(ctx->data);
            auto Fileid = std::make_shared<qint64>(ctx->fileId);
            auto Total = std::make_shared<qint64>(ctx->totalSize);
            if(ctx->type=="updateinformation"){
                connect(&sql,&LinkSQL::frienddata,this,[=](int friendid){
                    LinkSQL sql2;
                    if(sql2.isgroupchat(friendid)){
                        return;
                    }
                    QByteArray ctxdata=*array;
                    QMutexLocker locker(&idmarkmutex);
                    QString friendmark=idmarkmap[friendid];
                    QJsonObject json;
                    qint64 fileid1=*Fileid;
                    json["type"]="friendpicturechanged";
                    json["friendid"]=id;
                    json["fileid"]=*Fileid;
                    json["size"]=*Total;
                    QJsonDocument doc(json);
                    QByteArray m_data;
                    m_data = doc.toJson(QJsonDocument::Compact);
                    qint64 len = qToBigEndian<qint64>(m_data.size());
                    QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
                    newdata.append(m_data);
                    bool isnull=0;
                    socketmapmutex.lock();
                    if(socketmap[friendmark].msgworker!=NULL){
                        isnull=1;
                    }
                    socketmapmutex.unlock();
                    if(isnull){
                        int p=1;
                        while(p--){//避免死锁
                            QMutexLocker locker1(&socketmapmutex);
                            socketmap[friendmark].msgworker->otherthreadsendmsg(newdata);
                        }
                        QDataStream stream(&ctxdata, QIODevice::ReadOnly);
                        const qint64 CHUNK_SIZE = 1024 * 50;
                        while(!stream.atEnd()){
                            QByteArray newdata(CHUNK_SIZE, 0);
                            qint64 bytesRead = stream.readRawData(newdata.data(), CHUNK_SIZE);

                            if (bytesRead < CHUNK_SIZE) {
                                newdata.resize(bytesRead);
                            }
                            packingfile(newdata,fileid1,idmarkmap[friendid],friendid);
                        }
                    }
                });
                sql.selectfriend(id);
            }
            queue->removeAll(*ctx);

        }
    }
    usermutex->unlock();
}

void MsgWorker::addfriend(QString type, QString mark, int userid, int friendid, QString friendname, bool isgroupchat, int allsize, int cursize)
{
    qDebug()<<"进入初始化好友函数"<<'\n';
    QJsonObject json;
    json["type"]=type;
    if(type=="selectuser"){
        json["num"]=1;
    }
    json["allnum"]=allsize;
    json["curnum"]=cursize;
    json["id"]=userid;
    json["friendid"]=friendid;
    json["friendname"]=friendname;
    LinkSQL sql;
    QString path=sql.selecticon(friendid);
    qint64 filesize;
    QString iconpath="";
    qDebug()<<"path:"<<path<<'\n';
    if(path==""){
        iconpath="usericon/1.png";
    }else{
        iconpath="usericon/"+QString::number(friendid)+".png";
    }
    if(isgroupchat){
        iconpath="usericon/2.png";
    }
    QFile file(iconpath);
    file.open(QIODevice::ReadOnly);
    filesize=file.size();
    fileidmutex.lock();
    qint64 fileId = m_fileid++;
    if(m_fileid>1e15){
        m_fileid=0;
    }
    json["fileid"]=fileId;
    fileidmutex.unlock();
    json["size"]=filesize;
    socketmapmutex.lock();
    if(socketmap[mark].msgworker==NULL){
        qDebug()<<"id:"<<userid<<'\n';
        QJsonDocument doc(json);
        QByteArray data;
        data = doc.toJson(QJsonDocument::Compact);
        offlinemsgmapmutex.lock();
        vismapmutex.lock();
        if(vismap[userid]==0){
            B *b=new B;
            offlinemsgmap.insert(userid,b);
        }
        vismap[userid]=1;
        vismapmutex.unlock();
        offlinemsgmap[userid]->mutex->lock();
        QByteArray newdata;
        int p=1;
        newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
        newdata.append(data);
        offlinemsgmap[userid]->queue->enqueue(newdata);
        offlinemsgmap[userid]->mutex->unlock();
        offlinemsgmapmutex.unlock();
    }else{
        qDebug()<<"不为空"<<'\n';
        QJsonDocument doc(json);
        QByteArray m_data;
        m_data = doc.toJson(QJsonDocument::Compact);
        qint64 len = qToBigEndian<qint64>(m_data.size());
        QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
        newdata.append(m_data);
        socketmap[mark].msgworker->otherthreadsendmsg(newdata);
        //emit socketmap[mark].msgworker->senddatasignal(newdata);
    }
    socketmapmutex.unlock();
    const qint64 CHUNK_SIZE = 1024 * 50;
    while (!file.atEnd()) {
        QByteArray chunk = file.read(CHUNK_SIZE);
        packingfile(chunk,fileId,mark,userid);
    }
    file.close();
}

void MsgWorker::onlinesendmsg(int id)
{
    offlinemsgmapmutex.lock();
    QQueue<QByteArray>*idqueue=offlinemsgmap[id]->queue;
    if(idqueue==NULL){
        return;
    }
    offlinemsgmap[id]->mutex->lock();
    while(!idqueue->isEmpty()){
        QByteArray idarray=idqueue->front();
        idqueue->pop_front();
        int p=0;
        p = *reinterpret_cast<const int*>(idarray.constData());
        qDebug()<<"p:"<<p<<'\n';
        QByteArray chunk = idarray.mid(sizeof(int));
        if(p==1){
            QJsonDocument doc=QJsonDocument::fromJson(chunk);
            QJsonObject request = doc.object();
            QJsonDocument doc1(request);
            QByteArray m_data;
            m_data = doc1.toJson(QJsonDocument::Compact);
            qint64 len = qToBigEndian<qint64>(m_data.size());
            QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
            newdata.append(m_data);
            socketmapmutex.lock();
            idmarkmutex.lock();
            socketmap[idmarkmap[id]].msgworker->otherthreadsendmsg(newdata);
            //emit socketmap[idmarkmap[id]].msgworker->senddatasignal(newdata);
            idmarkmutex.unlock();
            socketmapmutex.unlock();
        }else if(p==2){
            sendonlinemsg(id,chunk);
        }
    }
    vismapmutex.lock();
    vismap[id]=0;
    vismapmutex.unlock();
    offlinemsgmap[id]->mutex->unlock();
    offlinemsgmap.remove(id);
    offlinemsgmapmutex.unlock();
}

void MsgWorker::sendonlinemsg(int id, QByteArray &data)
{
    socketmapmutex.lock();
    idmarkmutex.lock();
    if(socketmap[idmarkmap[id]].fileworker!=NULL){
        socketmap[idmarkmap[id]].fileworker->otherthreadsendmsg(data);
        //emit socketmap[idmarkmap[id]].fileworker->senddatasignal(data);
    }else{
        qDebug()<<"离线消息没有拿到socket"<<'\n';
    }
    idmarkmutex.unlock();
    socketmapmutex.unlock();
}

void MsgWorker::sendgroupchat(QList<int> &list, QString chatname, int groupchatid)
{
    for(int i=0;i<list.count();i++){
        QJsonObject json;
        json["type"]="creategroupchat";
        json["id"]=groupchatid;
        json["name"]=chatname;
        fileidmutex.lock();
        qint64 fileId = m_fileid++;
        if(m_fileid>1e15){
            m_fileid=0;
        }
        json["fileid"]=fileId;
        fileidmutex.unlock();
        idmarkmutex.lock();
        QString mark=idmarkmap[list[i]];
        idmarkmutex.unlock();
        socketmapmutex.lock();
        if(socketmap[mark].msgworker==NULL){
            socketmapmutex.unlock();
            continue;
        }
        qint64 filesize;
        QFile file("usericon/2.png");
        file.open(QIODevice::ReadOnly);
        filesize=file.size();
        json["size"]=filesize;
        QJsonDocument doc(json);
        QByteArray m_data;
        m_data = doc.toJson(QJsonDocument::Compact);
        qint64 len = qToBigEndian<qint64>(m_data.size());
        QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
        newdata.append(m_data);
        socketmap[mark].msgworker->otherthreadsendmsg(newdata);
        //emit socketmap[mark].msgworker->senddatasignal(newdata);
        socketmapmutex.unlock();
        const qint64 CHUNK_SIZE = 1024 * 50;
        while (!file.atEnd()) {
            QByteArray chunk = file.read(CHUNK_SIZE);
            packingfile(chunk,fileId,mark,list[i]);
        }
        file.close();
    }
    LinkSQL sql;
    for(int i=0;i<list.count();i++){
        for(int j=0;j<list.count();j++){
            QString name=sql.selectname(list[j]);
            bool p=1;
            if(sql.selecticon(list[j]).isEmpty()){
                p=0;
            }else{
                p=1;
            }
            initgroupchat(list[i],groupchatid,list[j],p,name);
        }
    }
    emit deletegroupdata();
}

void MsgWorker::initgroupchat(int myid, int groupchatid, int groupmemberid, bool path, QString groupmembername)
{
    qDebug()<<groupchatid<<"群"<<groupmemberid<<'\n';
    QJsonObject json;
    json["type"]="initgroupchat";
    json["groupchatid"]=groupchatid;
    json["groupmemberid"]=groupmemberid;
    json["groupmembername"]=groupmembername;
    fileidmutex.lock();
    qint64 fileId = m_fileid++;
    if(m_fileid>1e15){
        m_fileid=0;
    }
    json["fileid"]=fileId;
    fileidmutex.unlock();
    QString s="";
    if(path){
        s="usericon/"+QString::number(groupmemberid)+".png";
    }else{
        s="usericon/1.png";
    }
    QFile file(s);
    file.open(QIODevice::ReadOnly);
    json["size"]=file.size();
    idmarkmutex.lock();
    QString mark=idmarkmap[myid];
    idmarkmutex.unlock();
    socketmapmutex.lock();
    if(socketmap[mark].msgworker==NULL){
        socketmapmutex.unlock();
        return;
    }
    QJsonDocument doc(json);
    QByteArray m_data;
    m_data = doc.toJson(QJsonDocument::Compact);
    qint64 len = qToBigEndian<qint64>(m_data.size());
    QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
    newdata.append(m_data);
    socketmap[mark].msgworker->otherthreadsendmsg(newdata);
    //emit socketmap[mark].msgworker->senddatasignal(newdata);
    socketmapmutex.unlock();
    const qint64 CHUNK_SIZE = 1024 * 50;
    while (!file.atEnd()) {
        QByteArray chunk = file.read(CHUNK_SIZE);
        packingfile(chunk,fileId,mark,groupmemberid);
    }
    file.close();
}

void MsgWorker::sendgroupmark(QList<int> list, QString mark)
{
    QJsonObject json2;
    json2["type"]="setgroupchat";
    QJsonArray array;
    for(int i=0;i<list.count();i++){
        array.append(list[i]);
    }
    json2["groupid"]=array;
    QJsonDocument doc(json2);
    QByteArray m_data;
    m_data = doc.toJson(QJsonDocument::Compact);
    qint64 len = qToBigEndian<qint64>(m_data.size());
    QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
    newdata.append(m_data);
    socketmapmutex.lock();
    socketmap[mark].msgworker->otherthreadsendmsg(newdata);
    //emit socketmap[mark].msgworker->senddatasignal(newdata);
    socketmapmutex.unlock();
}

void MsgWorker::sendmsgingroup(int groupchatid, int sender, QString msg, int id)
{
    QJsonObject json;
    json["type"]="groupchatmsg";
    json["groupchatid"]=groupchatid;
    json["msg"]=msg;
    json["sender"]=sender;
    idmarkmutex.lock();
    QString mark=idmarkmap[id];
    idmarkmutex.unlock();
    socketmapmutex.lock();
    if(socketmap[mark].msgworker==NULL){
        offlinemsgmapmutex.lock();
        vismapmutex.lock();
        if(vismap[id]==0){
            B *b=new B;
            offlinemsgmap.insert(id,b);
        }
        vismap[id]=1;
        vismapmutex.unlock();
        offlinemsgmap[id]->mutex->lock();
        QByteArray newdata;
        int p=1;
        newdata.append(reinterpret_cast<const char*>(&p), sizeof(int));
        QJsonDocument doc(json);
        QByteArray m_data;
        m_data = doc.toJson(QJsonDocument::Compact);
        newdata.append(m_data);
        offlinemsgmap[id]->queue->enqueue(newdata);
        offlinemsgmap[id]->mutex->unlock();
        offlinemsgmapmutex.unlock();
    }else{
        QJsonDocument doc(json);
        QByteArray m_data;
        m_data = doc.toJson(QJsonDocument::Compact);
        qint64 len = qToBigEndian<qint64>(m_data.size());
        QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
        newdata.append(m_data);
        socketmap[mark].msgworker->otherthreadsendmsg(newdata);
        //emit socketmap[mark].msgworker->senddatasignal(newdata);
    }
    socketmapmutex.unlock();
}
//ctx
void MsgWorker::msgreaddata(QTcpSocket *msgtcp)
{
    while(msgtcp->bytesAvailable()>0){
        if(msgtcp->bytesAvailable()<static_cast<qint64>(sizeof(qint64))){
            return;
        }
        qint64 len;
        msgtcp->read(reinterpret_cast<char*>(&len), sizeof(len));
        len = qFromBigEndian(len);
        while (msgtcp->bytesAvailable() < len) {
            if (!msgtcp->waitForReadyRead(1000)) break;
        }
        QByteArray data = msgtcp->read(len);
        managejson(data,msgtcp);
    }
}
