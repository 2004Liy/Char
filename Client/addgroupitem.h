#ifndef ADDGROUPITEM_H
#define ADDGROUPITEM_H

#include <QWidget>

namespace Ui {
class Addgroupitem;
}

class Addgroupitem : public QWidget
{
    Q_OBJECT

public:
    explicit Addgroupitem(QWidget *parent = nullptr);
    ~Addgroupitem();

public:
    void init(QString name,QPixmap pix,int friendid);
    int getid();
    bool ischecked();
    void membermark();
    bool member();

private:
    int id;
    bool ismember=0;

private:
    Ui::Addgroupitem *ui;
};

#endif // ADDGROUPITEM_H
