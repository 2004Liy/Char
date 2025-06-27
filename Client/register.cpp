#include "register.h"
#include "ui_register.h"
#include<QMessageBox>
#include<QPainter>
#include<QGraphicsDropShadowEffect>

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(1.0);
    ui->pushButton_3->setStyleSheet(
        "QPushButton{"
        "border-image: url(:/icon/1.png);}"
        "QPushButton:hover {"
        "border-image: url(:/icon/2.png);"
        "}"
        );
    ui->pushButton->setStyleSheet(
        "QPushButton{"
        "background-color: rgb(85, 170, 255);"
        "font: 14pt \"Microsoft YaHei UI\";"
        "color: rgb(255, 255, 255);"
        "border-radius:6px;}"
        "QPushButton:hover {"
        "background-color: rgb(50, 131, 231);"
        "font: 14pt \"Microsoft YaHei UI\";"
        "color: rgb(255, 255, 255);"
        "border-radius:6px;"
        "}"
        );
}

Register::~Register()
{
    delete ui;
}

void Register::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(240,240,240));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}

void Register::on_pushButton_clicked()//点击注册
{
    QString name=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    if(name.size()>10||password.size()>20){
        QMessageBox::warning(this,"注意","用户名不要超过十位，密码不要超过二十位！！！");
        return;
    }
    if(name.size()==0||password.size()<3){
        QMessageBox::warning(this,"注意","用户名不能为空，密码不能少于三位！！！");
        return;
    }
    emit login(name,password);
}


void Register::on_pushButton_2_clicked()//点击返回登录
{
    emit closewidget();
}


void Register::on_pushButton_3_clicked()
{
    this->close();
}

