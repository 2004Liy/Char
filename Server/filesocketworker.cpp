#include "filesocketworker.h"
#include "linksql.h"

FileSocketWorker::FileSocketWorker(qintptr socketDescriptor):m_socketDescriptor(socketDescriptor){
    // setAutoDelete(true);
}

void FileSocketWorker::run()
{
    filesocket=new QTcpSocket();
    if (!filesocket->setSocketDescriptor(m_socketDescriptor)) {
        qWarning() << "Failed to set socket descriptor:" << filesocket->errorString();
        delete filesocket;
        filesocket = nullptr;
        return;
    }
    connect(filesocket,&QTcpSocket::readyRead,this,[=](){//有数据要接受
        qDebug()<<"-------------有新文件数据"<<'\n';
        filereaddata(filesocket);
    });
    QEventLoop loop;
    connect(filesocket, &QTcpSocket::disconnected, [&](){
        qDebug()<<"文件线程结束11111111111111"<<'\n';
        loop.quit();
    });
    loop.exec();
}

void FileSocketWorker::packingfile(const QByteArray &chunk, qint64 fileid, QString mark, int toid)
{
    QByteArray header;
    qint64 m_fileid=qToBigEndian<qint64>(fileid);
    qint64 len=qToBigEndian<qint64>(chunk.size());
    header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
    header.append(reinterpret_cast<const char*>(&len), sizeof(len));
    header.append(chunk);
    QTcpSocket *filetcp=socketmap[mark].filesocket;
    if(filetcp!=NULL){
        filetcp->write(header);
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
}

void FileSocketWorker::packintjson(const QJsonObject &json, QTcpSocket *msgtcp)
{
    QJsonDocument doc(json);
    QByteArray m_data;
    m_data = doc.toJson(QJsonDocument::Compact);
    qint64 len = qToBigEndian<qint64>(m_data.size());
    QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
    newdata.append(m_data);
    msgtcp->write(newdata);
    msgtcp->flush();
    qDebug()<<"json发送完成"<<'\n';
}

void FileSocketWorker::sendpix(int id, qint64 fileid, QByteArray data)
{
    QString filemark=idmarkmap[id];
    packingfile(data,fileid,filemark,id);
}

void FileSocketWorker::filereaddata(QTcpSocket *filetcp)
{
    while(filetcp->bytesAvailable()>0){
        if(filetcp->bytesAvailable()<static_cast<qint64>(sizeof(qint64)*2)){
            return;
        }
        qint64 mark;
        filetcp->read(reinterpret_cast<char*>(&mark), sizeof(mark));
        mark = qFromBigEndian(mark);
        if(mark==0){
            qint64 len;
            filetcp->read(reinterpret_cast<char*>(&len), sizeof(len));
            len = qFromBigEndian(len);
            while (filetcp->bytesAvailable() < len) {
                if (!filetcp->waitForReadyRead(1000)) break;
            }
            QByteArray data = filetcp->read(len);
            QJsonDocument doc=QJsonDocument::fromJson(data);
            QJsonObject request = doc.object();
            QString filemark = request["mark"].toString();
            QMutexLocker locker(&socketmapmutex);
            socketmap[filemark].filesocket=filetcp;
            qDebug()<<filemark<<"文件套接字绑定完成1111111111111111"<<"\n";

        }else if(mark==1){
            qint64 fileid;
            filetcp->read(reinterpret_cast<char*>(&fileid), sizeof(fileid));
            fileid=qFromBigEndian(fileid);
            qint64 id;
            filetcp->read(reinterpret_cast<char*>(&id), sizeof(id));
            id=qFromBigEndian(id);
            queuemapmutex.lock();
            QQueue<FileTransferContext>*queue=queuemap[id];
            queuemapmutex.unlock();
            qint64 len;
            filetcp->read(reinterpret_cast<char*>(&len), sizeof(len));
            len=qFromBigEndian(len);
            while (filetcp->bytesAvailable() < len) {
                if (!filetcp->waitForReadyRead(1000)) break;
            }
            QByteArray data = filetcp->read(len);

            FileTransferContext *ctx=NULL;
            for(auto &task:*queue){
                if(fileid==task.fileId){
                    ctx=&task;
                    break;
                }
            }
            if(ctx==NULL){//没有找到任务，在此创建任务
                FileTransferContext newctx;
                newctx.fileId=fileid;
                newctx.type="unknow";
                newctx.totalSize=-1;
                newctx.receivedSize=0;
                mutexmutex.lock();
                QMutex *usermutes=mutexmap[id];
                mutexmutex.unlock();
                usermutes->lock();
                queue->enqueue(newctx);
                ctx=&queue->last();
                usermutes->unlock();
            }
            QFile file("usericon/"+QString::number(id)+".png");
            if(ctx->receivedSize==0){
                file.open(QIODevice::WriteOnly);
            }else{
                file.open(QIODevice::WriteOnly|QIODevice::Append);
            }
            file.write(data);
            file.close();
            ctx->receivedSize+=data.size();
            ctx->data.append(data);
            if(ctx->totalSize>0&&ctx->receivedSize>=ctx->totalSize){
                mutexmutex.lock();
                QMutex *usermutes=mutexmap[id];
                mutexmutex.unlock();
                usermutes->lock();
                if(ctx->type=="updateinformation"){
                    LinkSQL sql;
                    connect(&sql,&LinkSQL::frienddata,this,[=](int friendid,QString friendname){
                        Q_UNUSED(friendname);
                        QByteArray ctxdata=ctx->data;
                        QString friendmark=idmarkmap[friendid];
                        QTcpSocket *msgtcp=socketmap[friendmark].msgsocket;
                        QJsonObject json;
                        json["type"]="friendpicturechanged";
                        json["friendid"]=id;
                        json["fileid"]=ctx->fileId;
                        json["size"]=ctx->totalSize;
                        if(msgtcp!=NULL){
                            packintjson(json,msgtcp);
                        }
                        QDataStream stream(&ctxdata, QIODevice::ReadOnly);
                        const qint64 CHUNK_SIZE = 1024 * 50;
                        qDebug()<<"发送头像文件"<<'\n';
                        while(!stream.atEnd()){
                            QByteArray newdata(CHUNK_SIZE, 0);
                            qint64 bytesRead = stream.readRawData(newdata.data(), CHUNK_SIZE);

                            if (bytesRead < CHUNK_SIZE) {
                                newdata.resize(bytesRead);
                            }
                            packingfile(newdata,ctx->fileId,idmarkmap[friendid],friendid);
                        }
                        qDebug()<<"发送完毕"<<'\n';
                    });
                    sql.selectfriend(id);
                }
                queue->removeAll(*ctx);
                usermutes->unlock();
            }
        }else if(mark==2){
            qint64 toid;
            filetcp->read(reinterpret_cast<char*>(&toid), sizeof(toid));
            toid = qFromBigEndian(toid);
            qDebug()<<"-------------------把信息转发给  "<<toid<<'\n';
            qint64 fileid;
            filetcp->read(reinterpret_cast<char*>(&fileid), sizeof(fileid));
            fileid=qFromBigEndian(fileid);
            qint64 len;
            filetcp->read(reinterpret_cast<char*>(&len), sizeof(len));
            len=qFromBigEndian(len);
            while (filetcp->bytesAvailable() < len) {
                if (!filetcp->waitForReadyRead(1000)) break;
            }
            QByteArray data = filetcp->read(len);
            QString filemark=idmarkmap[toid];
            packingfile(data,fileid,filemark,toid);
        }else if(mark==3){
            qint64 groupchatid;
            filetcp->read(reinterpret_cast<char*>(&groupchatid), sizeof(groupchatid));
            groupchatid = qFromBigEndian(groupchatid);
            qint64 senderid;
            filetcp->read(reinterpret_cast<char*>(&senderid), sizeof(senderid));
            senderid = qFromBigEndian(senderid);
            qint64 fileid;
            filetcp->read(reinterpret_cast<char*>(&fileid), sizeof(fileid));
            fileid=qFromBigEndian(fileid);
            qint64 len;
            filetcp->read(reinterpret_cast<char*>(&len), sizeof(len));
            len=qFromBigEndian(len);
            while (filetcp->bytesAvailable() < len) {
                if (!filetcp->waitForReadyRead(1000)) break;
            }
            QByteArray data = filetcp->read(len);
            LinkSQL sql;
            connect(&sql,&LinkSQL::groupmembersid,[=](int groupchat,int id,bool path,QString name){
                Q_UNUSED(groupchat);
                Q_UNUSED(path);
                Q_UNUSED(name);
                if(id!=senderid){
                    sendpix(id,fileid,data);
                }
            });
            sql.selectgroupmembers(groupchatid);
        }
    }
}
