#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include<QMouseEvent>
#include<QPoint>
#include<QListWidget>
#include<QLayout>
#include<QScrollArea>
#include<QVector>
#include<QTimer>
#include "prompt.h"
#include "attnitem.h"
#include "chatplaintext.h"
#include "mysendfile.h"
#include "friendsendfile.h"


namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

protected:
    void mousePressEvent(QMouseEvent *event)override;//鼠标按钮按下
    void mouseMoveEvent(QMouseEvent *event)override;//鼠标移动
    void mouseReleaseEvent(QMouseEvent *event) override;//鼠标按钮松开
    void closeEvent(QCloseEvent *event)override;

private:
    int id;
    QString name;
    QPixmap pix;
    bool isleft=0;
    QPoint point;
    QAction *action;
    QListWidget *friendwidget=NULL,*attnwidget=NULL,*selectfriend=NULL;
    QVBoxLayout *layout1=NULL;
    QWidget *curlistwidget=NULL;//记录当前是哪个列表
    QString m_path;
    QMap<int,QScrollArea*>chatmap;
    QVector<int>friendvector;//记录当前有那些好友
    QVector<int>promptvector;//记录当前有哪些好友申请
    QTimer timer;
    Prompt *prompt=NULL;
    ChatPlainText *chattext=NULL;
    QWidget *curchatscroll=NULL;
    int curfriendid;
    MySendfile *sendingfile=NULL;
    FriendSendfile *receivingfile=NULL;

    QHash<QString, QString>EMOJI_MAP = {
        {"😎", ":/icon/35.png"},
        {"😙", ":/icon/36.png"},
        {"😭", ":/icon/37.png"},
        {"😁", ":/icon/38.png"},
        {"😟", ":/icon/41.png"},
        {"🤬", ":/icon/40.png"}
    };

private:
    void widget_5addwidget(QListWidgetItem* item);
    void pushbuttoninit();
    bool promptdup(QVector<int>*vec,int dupid);
    void widget_5clear();
    void deleteid(int id);
    QListWidgetItem *findItemByAttnItem(QListWidget *listWidget, AttnItem *targetAttnItem);
    QString replaceEmojisWithImages(const QString& msg);

public slots:
    void on_logonsuccsee(int m_id,QString m_name);
    void on_logonsuccseeicon(QByteArray data);
    void friendwidgetadd(int id,QString name,QPixmap pix);
    void on_isnullid();
    void selectlistadd(int id, QString name, QPixmap pixmap);
    void on_addfriendrequest(int id, QString name, QPixmap pix);
    void on_receivesendmsg(int fromid,QString msg);
    void on_mysendsize(int value);
    void on_friendsendfilesize(int value);
    void on_frinedsendfile(qint64 size,QString filename,QString suffix,int fromid);
    void on_friendsendpicture(int fromid,QByteArray data,int h,int w);
    void on_friendpicturechanged(int friendid,QByteArray data);
    void on_friendnamechanged(int friendid ,QString friendname);
    void on_creategroupchat(QString name,int id,QByteArray data);
    void on_setgroupchat(QList<int>list);
    void on_addgroupmember(int groupchatid,int groupmemberid,QString groupmembername,QByteArray data);
    void on_groupchatmsg(int groupchatid,QString msg,int senderid);
    void on_sendpixingroup(int groupchatid,int senderid,QByteArray data,int h,int w);
    void on_ingroupsendfile(int senderid,qint64 size,QString filename,QString suffix,int groupid);

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_addfriend(int toid);

    void on_sendmsg(QString msg);

    void on_sendfile();

    void on_sendpicture();

    void on_pushButton_7_clicked();

    void on_addnewmember();

private:
    Ui::Home *ui;
public:signals:
    void updateinformation(int id,QString name,QString path);
    void updatenameonly(int id,QString name);
    void selectfriendid(int id);
    void addfriendrequest(int id);
    void agreeaddfriend(int id);
    void sendtofriend(int toid,QString msg,int fromid);
    void sendfiletofriend(int toid,QString path);
    void sendpicture(QString path,int toid);
    void closehome();
    void creategroup(QList<int> grouplist,QString name);
    void groupchatsign();
    void groupchataddmember(int groupchatid,QList<int>list);
    void sendmsgtogroupchat(int groupchatid,QString msg,int id);
    void sendpixtogroupchat(int groupchatid,QString path,int senderid);
    void sendfiletogroup(int groupid,QString path,int senderid);
};

#endif // HOME_H
