#include "mysendfile.h"
#include "ui_mysendfile.h"
#include<QFileInfo>

MySendfile::MySendfile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MySendfile)
{
    ui->setupUi(this);
}

MySendfile::~MySendfile()
{
    delete ui;
}


void MySendfile::setfileicon(QString filepath)
{
    QFileInfo fileinfo(filepath);
    ui->label_2->setText(fileinfo.fileName());
    qint64 size=fileinfo.size();
    double sizeKB=size/1024.0;
    if(sizeKB>=1024){
        double sizeMB=sizeKB/1024.0;
        if(sizeMB>=1024){
            double sizeGB=sizeMB/1024.0;
            ui->label_3->setText(QString::number(sizeGB)+"GB");
        }else{
            ui->label_3->setText(QString::number(sizeMB)+"MB");
        }
    }else{
        ui->label_3->setText(QString::number(sizeKB)+"KB");
    }
    QString suffix = fileinfo.suffix().toLower();
    QPixmap pix;
    if(suffix=="bmp"){
        pix=QPixmap(":/icon/20.png");
    }else if(suffix=="dos"){
        pix=QPixmap(":/icon/21.png");
    }else if(suffix=="eps"){
        pix=QPixmap(":/icon/22.png");
    }else if(suffix=="exe"){
        pix=QPixmap(":/icon/23.png");
    }else if(suffix=="html"){
        pix=QPixmap(":/icon/24.png");
    }else if(suffix=="mp4"){
        pix=QPixmap(":/icon/25.png");
    }else if(suffix=="pdf"){
        pix=QPixmap(":/icon/26.png");
    }else if(suffix=="ppt"){
        pix=QPixmap(":/icon/27.png");
    }else if(suffix=="psd"){
        pix=QPixmap(":/icon/28.png");
    }else if(suffix=="txt"){
        pix=QPixmap(":/icon/29.png");
    }else if(suffix=="xls"){
        pix=QPixmap(":/icon/30.png");
    }else if(suffix=="xml"){
        pix=QPixmap(":/icon/31.png");
    }else if(suffix=="zip"){
        pix=QPixmap(":/icon/34.png");
    }else if(suffix==""){
        pix=QPixmap(":/icon/33.png");
    }else{
        pix=QPixmap(":/icon/32.png");
    }
    ui->label->setText("");
    ui->label->setPixmap(pix);
    ui->label_4->setText("发送中");
}

void MySendfile::setvalue(int value)
{
    ui->progressBar->setValue(value);
    if(value==100){
        ui->label_4->setText("发送完毕");
    }
}

