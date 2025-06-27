#ifndef FILETRANSFERCONTEXT_H
#define FILETRANSFERCONTEXT_H
#include<QString>
class FileTransferContext{
public:
    QString type;
    qint64 fileId;//文件唯一id
    qint64 totalSize;//文件大小
    qint64 receivedSize;//当前接收大小
    QByteArray data;//文件内容
    int friendid;
    QString friendname;
    QString path;
    int h;
    int w;
    int groupchatid;

    bool operator==(const FileTransferContext &other)const{
        return this->fileId==other.fileId;
    }
};

class BeforFiledata{
public:
    qint64 fileid;
    QByteArray data;

    bool operator==(const BeforFiledata &other)const{
        if(this->fileid==other.fileid&&this->data==other.data){
            return 1;
        }else{
            return 0;
        }
    }
};

#endif // FILETRANSFERCONTEXT_H
