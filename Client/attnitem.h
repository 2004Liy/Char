#ifndef ATTNITEM_H
#define ATTNITEM_H

#include <QWidget>

namespace Ui {
class AttnItem;
}

class AttnItem : public QWidget
{
    Q_OBJECT

public:
    explicit AttnItem(QWidget *parent = nullptr);
    ~AttnItem();

private:
    int id;
    QString name;
    QPixmap pixmap;

public:
    void init(int id,QString name,QPixmap pix);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AttnItem *ui;

public:signals:
    void agreeclicked(int id,QString name,QPixmap pixmap);
    void disagree(int id);
};

#endif // ATTNITEM_H
