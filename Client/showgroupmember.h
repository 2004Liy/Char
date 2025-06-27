#ifndef SHOWGROUPMEMBER_H
#define SHOWGROUPMEMBER_H

#include <QWidget>

namespace Ui {
class ShowGroupmember;
}

class ShowGroupmember : public QWidget
{
    Q_OBJECT

public:
    explicit ShowGroupmember(QWidget *parent = nullptr);
    ~ShowGroupmember();
    void addbtn();
    void additem(QString name,QPixmap pix);

protected:
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event)override;

private:
    Ui::ShowGroupmember *ui;

public:signals:
    void closewidget();
    void addnewmember();
};

#endif // SHOWGROUPMEMBER_H
