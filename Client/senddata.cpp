#include "senddata.h"
#include<QtEndian>
#include<QDateTime>
#include<QFileInfo>
#include<QThread>
#include<QCoreApplication>
#include<QJsonArray>

SendData::SendData(QObject *parent)
    : QObject{parent}
{
    map=new QMap<QString,MyBase*>;
    connect(this,&SendData::runinit,this,&SendData::init);
    emit runinit();
    Register1* logon=new Register1(mark);
    connect(logon,&Register1::addfilequeuesignal,this,&SendData::addfilequeue);
    connect(logon,&Register1::logonsuccsee1,this,[this](int m_id,QString m_name){
        this->id=m_id;
        this->name=m_name;
        emit this->logonsuccsee(m_id,m_name);
    });
    connect(logon,&Register1::logonfailure,this,&SendData::logonfailure);
    connect(logon,&Register1::logoning,this,&SendData::logoning);
    map->insert("logon",logon);
    mylist.append(logon);

    Login* login=new Login(mark);
    connect(login,&Login::addfilequeuesignal,this,&SendData::addfilequeue);
    connect(login,&Login::loginsuccsee,this,[this](int m_id,QString m_name){
        this->id=m_id;
        this->name=m_name;
        emit this->logonsuccsee(m_id,m_name);
    });
    connect(login,&Login::loginfailure,this,&SendData::loginfailure);
    map->insert("login",login);
    mylist.append(login);

    InitfriendOragreeadd *initfr=new InitfriendOragreeadd(mark);
    connect(initfr,&InitfriendOragreeadd::addfilequeuesignal,this,&SendData::addfilequeue);
    connect(initfr,&InitfriendOragreeadd::senddata,this,[this](QByteArray& byte){
        filesocket->write(byte);
        filesocket->flush();
    });
    map->insert("initfriend",initfr);
    map->insert("agreeadd",initfr);
    mylist.append(initfr);

    Selectuser* selectuser=new Selectuser(mark);
    connect(selectuser,&Selectuser::addfilequeuesignal,this,&SendData::addfilequeue);
    connect(selectuser,&Selectuser::isnullid,this,&SendData::isnullid);
    map->insert("selectuser",selectuser);
    mylist.append(selectuser);

    Friendrequest *friendrequest=new Friendrequest(mark);
    connect(friendrequest,&Friendrequest::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("friendrequest",friendrequest);
    mylist.append(friendrequest);

    Sendmsg *sendmsg=new Sendmsg(mark);
    connect(sendmsg,&Sendmsg::receivesendmsg,this,&SendData::receivesendmsg);
    map->insert("sendmsg",sendmsg);
    mylist.append(sendmsg);

    Sendfile* sendfile=new Sendfile(mark);
    connect(sendfile,&Sendfile::frinedsendfile,this,&SendData::frinedsendfile);
    connect(sendfile,&Sendfile::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("sendfile",sendfile);
    mylist.append(sendfile);

    Sendpicture* sendpicture=new Sendpicture(mark);
    connect(sendpicture,&Sendpicture::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("sendpicture",sendpicture);
    mylist.append(sendpicture);

    Friendpicturechanged* friendpicturechanged=new Friendpicturechanged(mark);
    connect(friendpicturechanged,&Friendpicturechanged::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("friendpicturechanged",friendpicturechanged);
    mylist.append(friendpicturechanged);

    Friendnamechanged *friendnamechanged=new Friendnamechanged(mark);
    connect(friendnamechanged,&Friendnamechanged::friendnamechanged,this,&SendData::friendnamechanged);
    map->insert("friendnamechanged",friendnamechanged);
    mylist.append(friendnamechanged);

    Creategroupchat* creategroupchat=new Creategroupchat(mark);
    connect(creategroupchat,&Creategroupchat::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("creategroupchat",creategroupchat);
    mylist.append(creategroupchat);

    Setgroupchat* setgroupchat=new Setgroupchat(mark);
    connect(setgroupchat,&Setgroupchat::setgroupchat,this,&SendData::setgroupchat);
    map->insert("setgroupchat",setgroupchat);
    mylist.append(setgroupchat);

    Initgroupchat* initgroupchat=new Initgroupchat(mark);
    connect(initgroupchat,&Initgroupchat::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("initgroupchat",initgroupchat);
    mylist.append(initgroupchat);

    Groupchatmsg* groupchatmsg=new Groupchatmsg(mark);
    connect(groupchatmsg,&Groupchatmsg::groupchatmsg,this,&SendData::groupchatmsg);
    map->insert("groupchatmsg",groupchatmsg);
    mylist.append(groupchatmsg);

    Sendpixingroup* sendpixingroup=new Sendpixingroup(mark);
    connect(sendpixingroup,&Sendpixingroup::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("sendpixingroup",sendpixingroup);
    mylist.append(sendpixingroup);

    Sendfiletogroup* sendfiletogroup=new Sendfiletogroup(mark);
    connect(sendfiletogroup,&Sendfiletogroup::ingroupsendfile,this,&SendData::ingroupsendfile);
    connect(sendfiletogroup,&Sendfiletogroup::addfilequeuesignal,this,&SendData::addfilequeue);
    map->insert("sendfiletogroup",sendfiletogroup);
    mylist.append(sendfiletogroup);
}

SendData::~SendData()
{
    for(auto at:mylist){
        delete at;
    }
    delete map;
}


void SendData::init()
{
    mark=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");//生成客户端唯一标识
    udpsocket=new QUdpSocket(this);
    udpsocket->bind(QHostAddress::AnyIPv4, 45454,
                    QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress);
    connect(udpsocket,&QUdpSocket::readyRead,this,&SendData::getudpsocket);
}

void SendData::on_msgreadyRead()//处理msgsocket接收的消息
{
    qDebug()<<"收到msg数据"<<'\n';
    //QMutexLocker locker(&msgmutex);
    while(msgsocket->bytesAvailable()>0){
        if (msgsocket->bytesAvailable() < static_cast<qint64>(sizeof(qint64))){
            return;
        }
        qint64 len;
        msgsocket->read(reinterpret_cast<char*>(&len), sizeof(len));
        len = qFromBigEndian(len);
        qDebug()<<"len:"<<len<<'\n';
        while (msgsocket->bytesAvailable() < len) {
            qDebug()<<"等待..."<<'\n';
            if (!msgsocket->waitForReadyRead(1000)) break;
        }
        QByteArray data = msgsocket->read(len);
        msgqueue.enqueue(data);
        qDebug()<<"加入队列"<<'\n';
    }
    QMetaObject::invokeMethod(this, "handleMessages", Qt::QueuedConnection);//异步处理
}

void SendData::on_filereadyRead()//处理filesocket接收的消息
{
    //QMutexLocker locker(&filemutex);
    while(filesocket->bytesAvailable()>0){
        if(filesocket->bytesAvailable()<static_cast<qint64>(sizeof(qint64)*2)){
            return;
        }
        qint64 fileid;
        filesocket->read(reinterpret_cast<char*>(&fileid), sizeof(fileid));
        fileid = qFromBigEndian(fileid);
        qint64 len;
        filesocket->read(reinterpret_cast<char*>(&len), sizeof(len));
        len = qFromBigEndian(len);
        while (filesocket->bytesAvailable() < len) {
            if (!filesocket->waitForReadyRead(1000)) break;
        }

        QByteArray data = filesocket->read(len);
        if(fileid==-1){
            qDebug()<<"-------------------"<<"收到数据"<<'\n';
            continue;
        }
        FileTransferContext *ctx=NULL;
        QMutexLocker locker(&filemutex);
        for(auto &task:filequeue){
            if(fileid==task.fileId){
                ctx=&task;
                break;
            }
        }
        if(ctx==NULL){//如果没有找到任务，存入临时队列
            qDebug()<<"没有找到任务"<<'\n';
            beforfilemutex.lock();
            BeforFiledata filedata;
            filedata.fileid=fileid;
            qDebug()<<data.size()<<'\n';
            filedata.data.append(data);
            beforfilequeue.enqueue(filedata);
            beforfilemutex.unlock();
        }else{
            if(ctx->type=="sendfile"||ctx->type=="sendfiletogroup"){
                QFile file(ctx->path);
                file.open(QIODevice::WriteOnly|QIODevice::Append);
                ctx->receivedSize+=data.size();
                file.write(data);
                file.close();
                emit friendsendfilesize((int)(ctx->receivedSize*100/ctx->totalSize));
            }else{
                ctx->receivedSize+=data.size();
                ctx->data.append(data);
            }
            qDebug()<<ctx->receivedSize<<"/"<<ctx->totalSize<<'\n';
            if(ctx->totalSize>0&&ctx->receivedSize>=ctx->totalSize){
                if(ctx->type=="logon"){
                    emit logonsuccseeicon(ctx->data);
                }else if(ctx->type=="login"){
                    emit loginsuccseeicon(ctx->data);
                }else if(ctx->type=="initfriend"||ctx->type=="agreeadd"){
                    emit initfriend(ctx->friendid,ctx->friendname,ctx->data);
                    if(ctx->type=="initfriend"&&ctx->h==ctx->w&&ctx->h!=0){
                        getgroupchatmark();
                    }
                }else if(ctx->type=="selectuser"){
                    emit showselectuser(ctx->friendid,ctx->friendname,ctx->data);
                }else if(ctx->type=="friendrequest"){
                    emit showfriendrequest(ctx->friendid,ctx->friendname,ctx->data);
                }else if(ctx->type=="sendfile"||ctx->type=="sendfiletogroup"){
                    emit friendsendfilesize((int)(ctx->receivedSize*100/ctx->totalSize));
                }else if(ctx->type=="sendpicture"){
                    emit friendsendpicture(ctx->friendid,ctx->data,ctx->h,ctx->w);
                }else if(ctx->type=="friendpicturechanged"){
                    qDebug()<<"-----------更换头像"<<'\n';
                    emit friendpicturechanged(ctx->friendid,ctx->data);
                }else if(ctx->type=="creategroupchat"){
                    emit creategroupchat(ctx->friendname,ctx->friendid,ctx->data);
                }else if(ctx->type=="initgroupchat"){
                    emit addgroupmember(ctx->friendid,ctx->h,ctx->friendname,ctx->data);
                }else if(ctx->type=="sendpixingroup"){
                    emit sendpixingroup(ctx->groupchatid,ctx->friendid,ctx->data,ctx->h,ctx->w);
                }
                filequeue.removeAll(*ctx);
            }
        }
    }
}



void SendData::getudpsocket()
{
    while (udpsocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 port;
        udpsocket->readDatagram(datagram.data(), datagram.size(), &sender, &port);

        QString msg = QString::fromUtf8(datagram);
        if (msg.startsWith("SERVER:")) {
            QStringList parts = msg.split(':');
            if (parts.size() == 3) {
                QString serverIP = parts[1];
                int serverPort = parts[2].toInt();
                connectToServer(serverIP, serverPort);
            }
        }
    }
}



void SendData::connectToServer(QString ip, int port)
{
    if (!msgsocket&&!filesocket){
        msgsocket=new QTcpSocket(this);
        connect(msgsocket,&QTcpSocket::disconnected,this,[=](){
            msgsocket->deleteLater();
            msgsocket=NULL;
            emit deletesocket();
        });
        connect(msgsocket,&QTcpSocket::readyRead,this,&SendData::on_msgreadyRead);
        filesocket=new QTcpSocket(this);
        connect(filesocket,&QTcpSocket::disconnected,this,[=](){
            filesocket->deleteLater();
            filesocket=NULL;
            emit deletesocket();
        });
        connect(filesocket,&QTcpSocket::readyRead,this,&SendData::on_filereadyRead);
    }
    if (msgsocket->state() == QAbstractSocket::ConnectedState) {
        return;
    }
    msgsocket->connectToHost(ip, port);
    filesocket->connectToHost(ip,port+1);
    //客户端发送信号，让服务器对应的两个socket绑定
    QJsonObject json;
    json["type"]="connect";
    json["mark"]=mark;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);


    QJsonObject filejson;
    filejson["mark"]=mark;
    QJsonDocument filedoc(filejson);
    QByteArray filedata = filedoc.toJson(QJsonDocument::Compact);
    QByteArray newfiledata;
    qint64 len = qToBigEndian<qint64>(filedata.size());
    qint64 filemark = qToBigEndian<qint64>(0);
    newfiledata.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
    newfiledata.append(reinterpret_cast<const char*>(&len), sizeof(len));
    newfiledata.append(filedata);
    filesocket->write(newfiledata);
    qDebug()<<mark<<"标识符1111111111111111"<<"\n";
    // timer1=new QTimer(this);
    // connect(timer1,&QTimer::timeout,this,[=](){
    //     QByteArray data,data1;
    //     qint64 filemark1 = qToBigEndian<qint64>(5);
    //     qint64 len1 = qToBigEndian<qint64>(data1.size());
    //     data.append(reinterpret_cast<const char*>(&filemark1), sizeof(filemark1));
    //     data.append(reinterpret_cast<const char*>(&len1), sizeof(len1));
    //     data.append(data1);
    //     qDebug()<<filesocket->write(data)<<"发送状态"<<'\n';
    //     filesocket->flush();
    // });
    // timer1->start(1000);
}

void SendData::packingjson(const QByteArray &m_data)//把要发送的json打包成自定义的格式
{
    qint64 len = qToBigEndian<qint64>(m_data.size());//大端通信
    QByteArray data;
    data.append(reinterpret_cast<const char*>(&len), sizeof(len));
    data.append(m_data);
    msgsocket->write(data);
    //msgsocket->flush();
}

void SendData::handleMessages()//异步处理msgqueue中的数据
{
    while(!msgqueue.empty()){
        QByteArray data = msgqueue.front();
        msgqueue.pop_front();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        processJson(doc.object());
    }
}

void SendData::processJson(const QJsonObject &json)
{
    QString type=json["type"].toString();
    qDebug()<<"json到达  type:"<<type<<'\n';
    if(type=="sendfiletogroup"){
        (*map)[type]->mysetid(this->id);
    }
    (*map)[type]->manage(json,type);
}

void SendData::addfilequeue(qint64 fileid, QString type,qint64 size,int friendid,QString friendname,QString path,int h,int w,int groupid)
{
    qDebug()<<"json处理"<<'\n';
    FileTransferContext newctx;
    newctx.fileId=fileid;
    newctx.totalSize=size;
    newctx.type=type;
    newctx.receivedSize=0;
    newctx.data.reserve(size);
    newctx.friendid=friendid;
    newctx.friendname=friendname;
    newctx.h=h;
    newctx.w=w;
    newctx.groupchatid=groupid;
    newctx.path=path;
    BeforFiledata *filedata=NULL;
    beforfilemutex.lock();
    filedata=selectfileid(fileid);
    while(filedata!=NULL){
        if(newctx.type=="sendfile"||newctx.type=="sendfiletogroup"){
            QFile file(newctx.path);
            file.open(QIODevice::WriteOnly|QIODevice::Append);
            file.write(filedata->data);
            newctx.receivedSize+=filedata->data.size();
            emit friendsendfilesize((int)(newctx.receivedSize*100/newctx.totalSize));
        }else{
            newctx.receivedSize+=filedata->data.size();
            newctx.data.append(filedata->data);
        }
        beforfilequeue.removeAll(*filedata);
        filedata=selectfileid(fileid);
    }
    beforfilemutex.unlock();
    qDebug()<<"fileid:"<<newctx.fileId<<'\n';
    qDebug()<<"当前接收大小:"<<newctx.receivedSize<<'\n';
    qDebug()<<"总大小:"<<newctx.totalSize<<'\n';

    if(newctx.receivedSize>=newctx.totalSize){
        if(newctx.type=="logon"){
            emit logonsuccseeicon(newctx.data);
        }else if(newctx.type=="login"){
            emit loginsuccseeicon(newctx.data);
        }else if(newctx.type=="initfriend"||newctx.type=="agreeadd"){
            emit initfriend(newctx.friendid,newctx.friendname,newctx.data);
            if(newctx.type=="initfriend"&&newctx.h==newctx.w&&newctx.h!=0){
                getgroupchatmark();
            }
        }else if(newctx.type=="selectuser"){
            emit showselectuser(newctx.friendid,newctx.friendname,newctx.data);
        }else if(newctx.type=="friendrequest"){
            emit showfriendrequest(newctx.friendid,newctx.friendname,newctx.data);
        }else if(newctx.type=="sendfile"||newctx.type=="sendfiletogroup"){
            emit friendsendfilesize((int)(newctx.receivedSize*100/newctx.totalSize));
        }else if(newctx.type=="sendpicture"){
            emit friendsendpicture(newctx.friendid,newctx.data,newctx.h,newctx.w);
        }else if(newctx.type=="friendpicturechanged"){
            qDebug()<<"-----------更换头像"<<'\n';
            emit friendpicturechanged(newctx.friendid,newctx.data);
        }else if(newctx.type=="creategroupchat"){
            emit creategroupchat(newctx.friendname,newctx.friendid,newctx.data);
        }else if(newctx.type=="initgroupchat"){
            emit addgroupmember(newctx.friendid,newctx.h,newctx.friendname,newctx.data);
        }else if(newctx.type=="sendpixingroup"){
            emit sendpixingroup(newctx.groupchatid,newctx.friendid,newctx.data,newctx.h,newctx.w);
        }
    }else{
        qDebug()<<"加入任务队列"<<'\n';
        filemutex.lock();
        filequeue.enqueue(newctx);
        filemutex.unlock();
    }
}

BeforFiledata* SendData::selectfileid(qint64 fileid)
{
    for(auto &other:beforfilequeue){
        if(other.fileid==fileid){
            return &other;
        }
    }
    return NULL;
}

void SendData::getgroupchatmark()
{
    qDebug()<<"发送获取群聊id"<<'\n';
    QJsonObject json;
    json["type"]="getgroupchatmark";
    json["id"]=id;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}


void SendData::on_userlogon(int id, QString password)
{
    QJsonObject json;
    json["type"]="logon";
    json["mark"]=mark;
    json["id"]=id;
    json["password"]=password;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_login(QString name, QString password)
{
    qDebug()<<"注册"<<'\n';
    QJsonObject json;
    json["type"]="login";
    json["mark"]=mark;
    json["name"]=name;
    json["password"]=password;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_updateinformation(int id, QString name, QString path)//更新信息
{
    QJsonObject json;
    json["type"]="updateinformation";
    json["name"]=name;
    json["id"]=id;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    qint64 size=file.size();
    qint64 fileId = QDateTime::currentMSecsSinceEpoch();
    json["fileid"]=fileId;
    json["size"]=size;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);

    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        QByteArray header;
        qint64 filemark = qToBigEndian<qint64>(1);
        header.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
        qint64 m_fileid=qToBigEndian<qint64>(fileId);
        header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
        qint64 m_id=qToBigEndian<qint64>(id);
        header.append(reinterpret_cast<const char*>(&m_id), sizeof(m_id));
        qint64 len=qToBigEndian<qint64>(chunk.size());
        header.append(reinterpret_cast<const char*>(&len), sizeof(len));
        header.append(chunk);
        filesocket->write(header);
    }
    file.close();
}

void SendData::on_updatenameonly(int id, QString name)
{
    qDebug()<<"只修改名字"<<'\n';
    QJsonObject json;
    json["type"]="updatenameonly";
    json["id"]=id;
    json["name"]=name;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}



void SendData::on_selectfriendid(int selectid)
{
    QJsonObject json;
    json["type"]="selectuser";
    json["id"]=id;
    json["selectid"]=selectid;
    json["mark"]=mark;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}



void SendData::on_addfriendrequest(int toid)
{
    QJsonObject json;
    json["type"]="friendrequest";
    json["fromid"]=this->id;
    json["toid"]=toid;
    json["fromname"]=this->name;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}



void SendData::on_agreeaddfriend(int addid)
{
    QJsonObject json;
    json["type"]="agreeadd";
    json["fromid"]=this->id;
    json["fromname"]=this->name;
    json["toid"]=addid;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}



void SendData::on_sendtofriend(int toid, QString msg, int fromid)
{
    QJsonObject json;
    json["type"]="sendmsg";
    json["fromid"]=fromid;
    json["msg"]=msg;
    json["toid"]=toid;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}



void SendData::on_sendfiletofriend(int toid,QString path)
{
    QJsonObject json;
    json["type"]="sendfile";
    json["fromid"]=this->id;
    json["toid"]=toid;
    QFileInfo fileinfo(path);
    json["filename"]=fileinfo.fileName();
    json["suffix"]=fileinfo.suffix().toLower();
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    qint64 size=file.size();
    qint64 fileId = QDateTime::currentMSecsSinceEpoch();
    json["fileid"]=fileId;
    json["size"]=size;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
    qint64 sendsize=0;
    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        sendsize+=chunk.size();
        int lastProgress = -1;
        int currentProgress = (int)(sendsize * 100 / size);
        if (currentProgress != lastProgress) {
            emit mysendsize(currentProgress);
            lastProgress = currentProgress;
        }
        QByteArray header;
        qint64 filemark = qToBigEndian<qint64>(2);
        header.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
        qint64 m_id=qToBigEndian<qint64>(toid);
        header.append(reinterpret_cast<const char*>(&m_id), sizeof(m_id));
        qint64 m_fileid=qToBigEndian<qint64>(fileId);
        header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));

        qint64 len=qToBigEndian<qint64>(chunk.size());
        header.append(reinterpret_cast<const char*>(&len), sizeof(len));
        header.append(chunk);
        if(filesocket->state()== QAbstractSocket::ConnectedState){
            qDebug()<<"连接状态"<<'\n';
        }else{
            qDebug()<<"断开状态"<<'\n';
        }
        qint64 result=filesocket->write(header);
        if(result==-1){
            qDebug()<<"发送失败"<<'\n';
        }else{
            qDebug()<<"发送成功"<<'\n';
        }
        QCoreApplication::processEvents();
        QThread::usleep(100);
    }
    file.close();
}

void SendData::on_sendpicture(QString path,int toid)
{
    QJsonObject json;
    json["type"]="sendpicture";
    json["fromid"]=this->id;
    json["toid"]=toid;
    QPixmap pix(path);
    json["h"]=pix.height();
    json["w"]=pix.width();
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    qint64 size=file.size();
    qint64 fileId = QDateTime::currentMSecsSinceEpoch();
    json["fileid"]=fileId;
    json["size"]=size;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);

    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        QByteArray header;
        qint64 filemark = qToBigEndian<qint64>(2);
        header.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
        qint64 m_id=qToBigEndian<qint64>(toid);
        header.append(reinterpret_cast<const char*>(&m_id), sizeof(m_id));
        qint64 m_fileid=qToBigEndian<qint64>(fileId);
        header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
        qint64 len=qToBigEndian<qint64>(chunk.size());
        header.append(reinterpret_cast<const char*>(&len), sizeof(len));
        header.append(chunk);
        filesocket->write(header);
    }
    file.close();
}

void SendData::on_closehome()
{
    QJsonObject json;
    json["type"]="close";
    json["mark"]=mark;
    json["id"]=this->id;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_creategroup(QList<int> list, QString groupname)
{
    QJsonObject json;
    json["type"]="creategroup";
    json["name"]=groupname;
    QJsonArray jsonarray;
    for(int i=0;i<list.count();i++){
        jsonarray.append(list[i]);
    }
    json["memberid"]=jsonarray;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
    qDebug()<<"客户端发送创建群聊信息------------------"<<'\n';
}

void SendData::on_groupchatsign()
{
    qDebug()<<"请求群成员"<<'\n';
    QJsonObject json;
    json["type"]="getmember";
    json["id"]=id;
    json["mark"]=mark;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_groupchataddmember(int groupchatid, QList<int> list)
{
    QJsonObject json;
    json["type"]="addnewmember";
    json["groupchatid"]=groupchatid;
    QJsonArray array;
    for(int i=0;i<list.count();i++){
        array.append(list[i]);
    }
    json["newmember"]=array;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_sendmsgtogroupchat(int groupchatid, QString msg, int id)
{
    QJsonObject json;
    json["type"]="sendmsgingroup";
    json["groupchatid"]=groupchatid;
    json["msg"]=msg;
    json["sender"]=id;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
}

void SendData::on_sendpixtogroupchat(int groupchatid, QString path, int senderid)
{
    QJsonObject json;
    json["type"]="sendpixingroup";
    json["groupchatid"]=groupchatid;
    json["senderid"]=senderid;
    QPixmap pix(path);
    json["h"]=pix.height();
    json["w"]=pix.width();
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    qint64 size=file.size();
    qint64 fileId = QDateTime::currentMSecsSinceEpoch();
    json["fileid"]=fileId;
    json["size"]=size;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        QByteArray header;
        qint64 filemark = qToBigEndian<qint64>(3);
        header.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
        qint64 m_id=qToBigEndian<qint64>(groupchatid);
        header.append(reinterpret_cast<const char*>(&m_id), sizeof(m_id));
        qint64 sender=qToBigEndian<qint64>((qint64)senderid);
        header.append(reinterpret_cast<const char*>(&sender), sizeof(sender));
        qint64 m_fileid=qToBigEndian<qint64>(fileId);
        header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
        qint64 len=qToBigEndian<qint64>(chunk.size());
        header.append(reinterpret_cast<const char*>(&len), sizeof(len));
        header.append(chunk);
        filesocket->write(header);
    }
    file.close();
}

void SendData::on_sendfiletogroup(int groupid, QString path, int senderid)
{
    QJsonObject json;
    json["type"]="sendfiletogroup";
    json["groupchatid"]=groupid;
    json["senderid"]=senderid;
    QFileInfo fileinfo(path);
    json["filename"]=fileinfo.fileName();
    json["suffix"]=fileinfo.suffix().toLower();
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    qint64 size=file.size();
    qint64 fileId = QDateTime::currentMSecsSinceEpoch();
    json["fileid"]=fileId;
    json["size"]=size;
    QJsonDocument doc(json);
    QByteArray m_data = doc.toJson(QJsonDocument::Compact);
    packingjson(m_data);
    qint64 sendsize=0;
    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        sendsize+=chunk.size();
        int lastProgress = -1;
        int currentProgress = (int)(sendsize * 100 / size);
        if (currentProgress != lastProgress) {
            emit mysendsize(currentProgress);
            lastProgress = currentProgress;
        }
        QByteArray header;
        qint64 filemark = qToBigEndian<qint64>(3);
        header.append(reinterpret_cast<const char*>(&filemark), sizeof(filemark));
        qint64 m_id=qToBigEndian<qint64>(groupid);
        header.append(reinterpret_cast<const char*>(&m_id), sizeof(m_id));
        qint64 sender=qToBigEndian<qint64>((qint64)senderid);
        header.append(reinterpret_cast<const char*>(&sender), sizeof(sender));
        qint64 m_fileid=qToBigEndian<qint64>(fileId);
        header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
        qint64 len=qToBigEndian<qint64>(chunk.size());
        header.append(reinterpret_cast<const char*>(&len), sizeof(len));
        header.append(chunk);
        filesocket->write(header);
    }
    file.close();
}

