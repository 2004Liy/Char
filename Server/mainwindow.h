#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include<QUdpSocket>
#include<QMutex>
#include<QMap>
#include<QList>
#include<QTimer>
#include<QJsonObject>
#include<QJsonDocument>
#include<QQueue>
#include<FileTransferContext.h>
#include<QThreadPool>
#include "mytcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    unsigned int port=1210;
    MyTcpServer *msgserver=NULL;
    MyTcpServer *fileserver=NULL;
    QString ip;
    QTimer *timer=NULL;

private:




private slots:
    void do_msgnewConnection(qintptr socket);
    void do_filenewConnection(qintptr socket);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
