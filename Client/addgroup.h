#ifndef ADDGROUP_H
#define ADDGROUP_H

#include <QWidget>

namespace Ui {
class Addgroup;
}

class Addgroup : public QWidget
{
    Q_OBJECT

public:
    explicit Addgroup(QWidget *parent = nullptr);
    ~Addgroup();

protected:
    void paintEvent(QPaintEvent *event)override;

public:
    void additem(QPixmap pix,QString name,int id,bool p);
    void changelabel(QString name);

private:
    Ui::Addgroup *ui;
signals:
    void closewidget();
    void creategroupchat(QList<int>list,QString name);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // ADDGROUP_H
