#include "stickerwidget.h"
#include "ui_stickerwidget.h"
#include<QGraphicsDropShadowEffect>

Stickerwidget::Stickerwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Stickerwidget)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //设置无边框
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("#444444"));
    //设置阴影圆角
    shadow->setBlurRadius(30);
    //给嵌套QWidget设置阴影
    ui->widget->setGraphicsEffect(shadow);
}

Stickerwidget::~Stickerwidget()
{
    delete ui;
}

void Stickerwidget::leaveEvent(QEvent *event)
{
    emit focusOut();
    this->close();
    QWidget::leaveEvent(event);
}



void Stickerwidget::on_pushButton_clicked()
{
    emit sticker1();
}


void Stickerwidget::on_pushButton_2_clicked()
{
    emit sticker2();
}


void Stickerwidget::on_pushButton_3_clicked()
{
    emit sticker3();
}


void Stickerwidget::on_pushButton_6_clicked()
{
    emit sticker4();
}


void Stickerwidget::on_pushButton_7_clicked()
{
    emit sticker5();
}


void Stickerwidget::on_pushButton_8_clicked()
{
    emit sticker6();
}

