#ifndef FRIENDSENDFILE_H
#define FRIENDSENDFILE_H

#include <QWidget>

namespace Ui {
class FriendSendfile;
}

class FriendSendfile : public QWidget
{
    Q_OBJECT

public:
    explicit FriendSendfile(QWidget *parent = nullptr);
    ~FriendSendfile();

    void setfile(QString filename,qint64 size,QString suffix);
    void setvalue(int value);

private:
    Ui::FriendSendfile *ui;
};

#endif // FRIENDSENDFILE_H
