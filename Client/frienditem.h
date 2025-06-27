#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    struct A{
        int id;
        QString name;
        QPixmap pix;
    };
    explicit FriendItem(QWidget *parent = nullptr);
    ~FriendItem();
    int id;
    QString name;
    QPixmap m_pix;
    bool isgroup=0;
    QList<A>list;
    QSet<int>set;

public:
    void init(int id,QString name,QPixmap pix);
    int getid();
    QString getname();
    QPixmap getpixmap();
    void havemsg(bool isok);
    void setmsg(QString msg,QString curtime);
    void picturechanged(QPixmap pix);
    void namechanged(QString name);
    void setisgroup();
    void addlist(int id,QString name,QPixmap pix);
    bool groupchat();
    int getlistcount();
    QString getlistnumname(int i);
    QPixmap getlistnumpix(int i);
    int getlistnumid(int i);
    QPixmap geticon(int id);
    QString idgetname(int id);
private:
    Ui::FriendItem *ui;
};

#endif // FRIENDITEM_H
