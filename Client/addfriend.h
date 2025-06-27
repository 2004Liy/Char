#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
    Q_OBJECT

public:
    int id;
    explicit AddFriend(QWidget *parent = nullptr);
    ~AddFriend();

public slots:
    void initwidget(int id,QString name,QPixmap pixmap);

private slots:
    void on_pushButton_clicked();

private:
    Ui::AddFriend *ui;
public:signals:
    void addfriend(int id);
};

#endif // ADDFRIEND_H
