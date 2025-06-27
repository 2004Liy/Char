#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include<QThread>
#include<QTcpSocket>
#include "baseworker.h"
#include<QTimer>
class FileWorker : public QThread,BaseWorker
{
    Q_OBJECT
public:
    explicit FileWorker(qintptr sock,QObject *parent = nullptr);

    void disconnect()override;

    ~FileWorker();

protected:
    void run()override;

private:
    void packingfile(const QByteArray &chunk, qint64 fileid, QString mark,int toid);
    void sendpix(int id, qint64 fileid, QByteArray data);

private:
    QTcpSocket *filesocket=NULL;
    qintptr m_sock;
    QTimer *timer;

private slots:
    void filereaddata(QTcpSocket *filetcp);

public slots:
    void otherthreadsendmsg( QByteArray header)override;

public:signals:
    void deletethread();
    void senddatasignal(QByteArray data)override;
};

#endif // FILEWORKER_H
