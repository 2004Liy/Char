#ifndef ISFRIEND_H
#define ISFRIEND_H

#include <QWidget>

namespace Ui {
class IsFriend;
}

class IsFriend : public QWidget
{
    Q_OBJECT

public:
    explicit IsFriend(QWidget *parent = nullptr);
    ~IsFriend();
private:
    int id;
    QString name;

public:
    void init(int id,QString name,QPixmap pix);

private:
    Ui::IsFriend *ui;
};

#endif // ISFRIEND_H
