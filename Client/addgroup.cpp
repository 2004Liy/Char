#include "addgroup.h"
#include "ui_addgroup.h"
#include<QPainter>
#include "addgroupitem.h"
#include<QVBoxLayout>
#include<QMessageBox>
#include<QTimer>

Addgroup::Addgroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addgroup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout *layout=new QVBoxLayout(ui->widget_2);
    layout->setSpacing(0);
    layout->addStretch();
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Addgroup::~Addgroup()
{
    delete ui;
}

void Addgroup::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}

void Addgroup::additem(QPixmap pix, QString name, int id,bool p)
{
    Addgroupitem *additem=new Addgroupitem(this);
    additem->init(name,pix,id);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->widget_2->layout());
    mainLayout->insertWidget(mainLayout->count()-1,additem);
    if(p){
        additem->membermark();
    }
}

void Addgroup::changelabel(QString name)
{
    ui->label->setText("邀请新成员");
    ui->lineEdit->setText(name);
    ui->lineEdit->setEnabled(0);
}

void Addgroup::on_pushButton_2_clicked()
{
    emit closewidget();
}


void Addgroup::on_pushButton_clicked()
{
    QString s=ui->lineEdit->text();
    if(s.isEmpty()){
        QMessageBox::information(this, "提示", "群聊名不能为空");
        return;
    }
    int sum=0;
    QList<int>groupchatid;
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(ui->widget_2->layout());
    for(int i=0;i<layout->count()-1;i++){
        QLayoutItem *item=layout->itemAt(i);
        QWidget *widget=item->widget();
        Addgroupitem *additem=qobject_cast<Addgroupitem*>(widget);
        if(additem->ischecked()&&additem->member()==0){
            sum++;
            groupchatid<<additem->getid();
        }
    }
    if(sum==0){
        QMessageBox::information(this, "提示", "请选择群聊成员");
        return;
    }
    emit creategroupchat(groupchatid,s);
    QTimer *timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        emit closewidget();
    });
    timer->start(200);
}

