#include "fileworker.h"
#include "linksql.h"

FileWorker::FileWorker(qintptr sock,QObject *parent)
    : QThread{parent},m_sock(sock)
{
}

void FileWorker::otherthreadsendmsg(QByteArray data)
{
    if (filesocket && filesocket->state() == QAbstractSocket::ConnectedState) {
        qDebug()<<"在对应线程"<<'\n';
        filesocket->write(data);
        filesocket->flush();
    }else{
        if(filesocket==NULL){
            qDebug()<<"为空"<<'\n';
        }else{
            qDebug()<<"不在对应线程"<<'\n';
        }
    }
}

void FileWorker::disconnect()
{
    filesocket->disconnectFromHost();
}

FileWorker::~FileWorker()
{
    if(filesocket){
        filesocket->deleteLater();
    }
}

void FileWorker::run()
{

    filesocket=new QTcpSocket;
    filesocket->setSocketDescriptor(m_sock);
    connect(this,&FileWorker::senddatasignal,this,&FileWorker::otherthreadsendmsg,Qt::DirectConnection);

    connect(filesocket,&QTcpSocket::readyRead,this,[this](){//有数据要接受
        qDebug()<<"-------------有新文件数据"<<'\n';
        filereaddata(filesocket);
    },Qt::DirectConnection);
    connect(filesocket, &QTcpSocket::disconnected, this,[this](){
        qDebug()<<"文件线程结束11111111111111"<<'\n';
        quit();
        this->wait();
        emit deletethread();
    },Qt::DirectConnection);
    exec();

}

void FileWorker::packingfile(const QByteArray &chunk, qint64 fileid, QString mark, int toid)
{
    QByteArray header;
    qint64 m_fileid=qToBigEndian<qint64>(fileid);
    qint64 len=qToBigEndian<qint64>(chunk.size());
    header.append(reinterpret_cast<const char*>(&m_fileid), sizeof(m_fileid));
    header.append(reinterpret_cast<const char*>(&len), sizeof(len));
    header.append(chunk);
    socketmapmutex.lock();
    if(socketmap[mark].fileworker!=NULL){
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

void FileWorker::sendpix(int id, qint64 fileid, QByteArray data)
{
    QString filemark=idmarkmap[id];
    packingfile(data,fileid,filemark,id);
}

void FileWorker::filereaddata(QTcpSocket *filetcp)
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
            socketmap[filemark].fileworker=this;
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
                    connect(&sql,&LinkSQL::frienddata,this,[=](int friendid){
                        QByteArray ctxdata=ctx->data;
                        idmarkmutex.lock();
                        QString friendmark=idmarkmap[friendid];
                        idmarkmutex.unlock();
                        QJsonObject json;
                        json["type"]="friendpicturechanged";
                        json["friendid"]=id;
                        json["fileid"]=ctx->fileId;
                        json["size"]=ctx->totalSize;
                        QJsonDocument doc(json);
                        QByteArray m_data;
                        m_data = doc.toJson(QJsonDocument::Compact);
                        qint64 len = qToBigEndian<qint64>(m_data.size());
                        QByteArray newdata(reinterpret_cast<const char*>(&len), sizeof(len));
                        newdata.append(m_data);
                        socketmapmutex.lock();
                        if(socketmap[friendmark].msgworker!=NULL){
                            socketmap[friendmark].msgworker->otherthreadsendmsg(newdata);
                        }
                        socketmapmutex.unlock();
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
            connect(&sql,&LinkSQL::groupmembersid,[=](int groupchat,int id){
                Q_UNUSED(groupchat);
                if(id!=senderid){
                    sendpix(id,fileid,data);
                }
            });
            sql.selectgroupmembers(groupchatid);
        }else if(mark==5){
            qDebug()<<"--------------连续数据"<<'\n';
            qint64 len;
            filetcp->read(reinterpret_cast<char*>(&len), sizeof(len));
            len = qFromBigEndian(len);
            while (filetcp->bytesAvailable() < len) {
                if (!filetcp->waitForReadyRead(1000)) break;
            }
            QByteArray data = filetcp->read(len);
        }
    }
}
