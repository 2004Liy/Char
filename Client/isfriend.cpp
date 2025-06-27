#include "isfriend.h"
#include "ui_isfriend.h"
#include<QPainter>

IsFriend::IsFriend(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IsFriend)
{
    ui->setupUi(this);
}

IsFriend::~IsFriend()
{
    delete ui;
}


void IsFriend::init(int id, QString name, QPixmap pix)
{
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
    ui->label_2->setText(name);
}

