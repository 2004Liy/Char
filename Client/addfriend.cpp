#include "addfriend.h"
#include "ui_addfriend.h"
#include<QPainter>

AddFriend::AddFriend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

AddFriend::~AddFriend()
{
    delete ui;
}


void AddFriend::initwidget(int id, QString name, QPixmap pixmap)
{
    pixmap = pixmap.scaled(ui->label->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);  // 缩放图片以适应标签大小
    QPixmap circularPixmap(ui->label->size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, ui->label->width(), ui->label->height());
    ui->label->setPixmap(circularPixmap);
    ui->label->setAlignment(Qt::AlignCenter);
    this->id=id;
    ui->label_2->setText(name);
}


void AddFriend::on_pushButton_clicked()
{
    emit addfriend(id);
}

