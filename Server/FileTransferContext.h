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

    bool operator==(const FileTransferContext &other)const{
        return this->fileId==other.fileId;
    }
};

#endif // FILETRANSFERCONTEXT_H
