#include "attnitem.h"
#include "ui_attnitem.h"
#include<QPainter>

AttnItem::AttnItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AttnItem)
{
    ui->setupUi(this);
    ui->pushButton->setStyleSheet(
        "QPushButton{"
        "border-radius:3px;"
        "background-color: rgb(85, 170, 255);"
        "color: rgb(0, 0, 0);}"
        "QPushButton:hover {"
        "background-color: rgb(50, 131, 231);"
        "border-radius:3px;"
        "color: rgb(0, 0, 0);"
        "}"
        );
    ui->pushButton_2->setStyleSheet(
        "QPushButton{"
        "border-radius:3px;"
        "background-color: rgb(204, 0, 0);"
        "color: rgb(0, 0, 0);}"
        "QPushButton:hover {"
        "background-color: rgb(160, 0, 0);"
        "border-radius:3px;"
        "color: rgb(0, 0, 0);"
        "}"
        );
}

AttnItem::~AttnItem()
{
    delete ui;
}


void AttnItem::init(int id, QString name, QPixmap pix)
{
    ui->label_2->setText(name+"请求添加好友");
    this->name=name;
    this->id=id;
    QPixmap pixmap = pix.scaled(ui->label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);  // 缩放图片以适应标签大小
    QPixmap circularPixmap(ui->label->size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, ui->label->width(), ui->label->height());
    ui->label->setPixmap(circularPixmap);
    ui->label->setAlignment(Qt::AlignCenter);
    this->pixmap=pix;
}


void AttnItem::on_pushButton_clicked()//点击同意
{
    agreeclicked(this->id,this->name,this->pixmap);
}


void AttnItem::on_pushButton_2_clicked()//不同意
{
    emit disagree(this->id);
}

