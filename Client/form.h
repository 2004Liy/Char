#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include"senddata.h"
#include<QThread>
#include "home.h"
#include "register.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

private:
    void init();

protected:
    void paintEvent(QPaintEvent *event)override;

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    SendData *senddata=NULL;
    QThread *subthread=NULL;
    Home *home=NULL;
    Register *regither=NULL;
    int close_num=0;

private:
    Ui::Form *ui;

public:signals:
    void senddatainit();
    void userlogon(int id,QString password);
};

#endif // FORM_H
