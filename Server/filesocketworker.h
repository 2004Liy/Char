#ifndef FILESOCKETWORKER_H
#define FILESOCKETWORKER_H

#include <QObject>
#include <QThread>
#include "baseworker.h"

class FileSocketWorker : public QThread,BaseWorker
{
    Q_OBJECT
private:
    QTcpSocket *filesocket=NULL;
    qintptr m_socketDescriptor;
public:
    FileSocketWorker(qintptr socketDescriptor);
protected:
    void run()override;

private:
    void packingfile(const QByteArray &chunk, qint64 fileid, QString mark,int toid);
    void packintjson(const QJsonObject &json, QTcpSocket *msgtcp);
    void sendpix(int id, qint64 fileid, QByteArray data);


private slots:
    void handleWriteRequest(QByteArray data, QString mark);

private slots:
    void filereaddata(QTcpSocket *filetcp);
};

#endif // FILESOCKETWORKER_H
