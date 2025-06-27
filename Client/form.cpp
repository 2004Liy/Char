#include "form.h"
#include "ui_form.h"
#include<QPainter>
#include<QMessageBox>
#include<QGraphicsDropShadowEffect>
#include<QPainterPath>

Form::Form(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
    init();
    home=new Home;
    home->hide();
    regither=new Register;
    regither->hide();

    subthread=new QThread;
    senddata=new SendData;
    senddata->moveToThread(subthread);
    connect(this,&Form::senddatainit,senddata,&SendData::init);
    connect(this,&Form::userlogon,senddata,&SendData::on_userlogon);//用户登录的槽函数
    connect(regither,&Register::login,senddata,&SendData::on_login);//用户注册
    connect(home,&Home::updateinformation,senddata,&SendData::on_updateinformation);//更新信息
    connect(home,&Home::updatenameonly,senddata,&SendData::on_updatenameonly);//只更新名字
    subthread->start();

    emit senddatainit();

    connect(senddata,&SendData::logonfailure,this,[=](){
        QMessageBox::warning(this,"注意","账号或密码错误");
    });
    connect(senddata,&SendData::logoning,this,[=](){
        QMessageBox::warning(this,"注意","此账号正在其他设备登录");
    });
    connect(senddata,&SendData::logonsuccsee,home,&Home::on_logonsuccsee);//设置home的id和名字
    connect(senddata,&SendData::logonsuccseeicon,this,[=](QByteArray data){//设置home界面头像
        home->on_logonsuccseeicon(data);
        home->show();
        regither->close();
        this->close();
    });
    connect(senddata,&SendData::loginfailure,this,[=](){
        QMessageBox::warning(this,"注意","注册失败，请重新尝试");
    });
    connect(senddata,&SendData::loginsuccsee,home,&Home::on_logonsuccsee);//用户注册设置home的id和名字
    connect(senddata,&SendData::loginsuccseeicon,this,[=](QByteArray data){//用户注册设置home界面头像
        home->on_logonsuccseeicon(data);
        home->show();
        regither->close();
        this->close();
    });
    connect(senddata,&SendData::initfriend,this,[=](int friendid,QString friendname,QByteArray data){
        QPixmap pix;
        pix.loadFromData(data);
        home->friendwidgetadd(friendid,friendname,pix);
    });
    connect(home,&Home::selectfriendid,senddata,&SendData::on_selectfriendid);//通过id查询用户
    connect(senddata,&SendData::isnullid,home,&Home::on_isnullid);//查询的id为空
    connect(senddata,&SendData::showselectuser,this,[=](int friendid,QString friendname,QByteArray data){
        QPixmap pix;
        pix.loadFromData(data);
        home->selectlistadd(friendid,friendname,pix);
    });
    connect(home,&Home::addfriendrequest,senddata,&SendData::on_addfriendrequest);//提示有好友申请
    connect(senddata,&SendData::showfriendrequest,this,[=](int friendid,QString friendname,QByteArray data){
        QPixmap pix;
        pix.loadFromData(data);
        home->on_addfriendrequest(friendid,friendname,pix);
    });
    connect(home,&Home::agreeaddfriend,senddata,&SendData::on_agreeaddfriend);//同意添加好友
    connect(home,&Home::sendtofriend,senddata,&SendData::on_sendtofriend);//发普通消息
    connect(senddata,&SendData::receivesendmsg,home,&Home::on_receivesendmsg);//收到别人发来的消息
    connect(home,&Home::sendfiletofriend,senddata,&SendData::on_sendfiletofriend);//发文件
    connect(senddata,&SendData::mysendsize,home,&Home::on_mysendsize);//设置自己传输文件进度条
    connect(senddata,&SendData::frinedsendfile,home,&Home::on_frinedsendfile);//收到好友发文件的信号创建对方发文件的widget
    connect(senddata,&SendData::friendsendfilesize,home,&Home::on_friendsendfilesize);//设置接收文件进度条
    connect(home,&Home::sendpicture,senddata,&SendData::on_sendpicture);//发送图片
    connect(senddata,&SendData::friendsendpicture,home,&Home::on_friendsendpicture);//收到朋友发来的图片
    connect(senddata,&SendData::friendpicturechanged,home,&Home::on_friendpicturechanged);//收到朋友改变头像
    connect(senddata,&SendData::friendnamechanged,home,&Home::on_friendnamechanged);//收到好友改变昵称的信号槽
    connect(home,&Home::creategroup,senddata,&SendData::on_creategroup);//创建群聊
    connect(senddata,&SendData::creategroupchat,home,&Home::on_creategroupchat);//服务器返回创建群聊的信息
    connect(senddata,&SendData::setgroupchat,home,&Home::on_setgroupchat);//客户端标记群聊
    connect(home,&Home::groupchatsign,senddata,&SendData::on_groupchatsign);//请求群聊成员
    connect(senddata,&SendData::addgroupmember,home,&Home::on_addgroupmember);//设置群成员
    connect(home,&Home::groupchataddmember,senddata,&SendData::on_groupchataddmember);//往群聊中加入新成员
    connect(home,&Home::sendmsgtogroupchat,senddata,&SendData::on_sendmsgtogroupchat);//在群聊中发消息
    connect(senddata,&SendData::groupchatmsg,home,&Home::on_groupchatmsg);//收到群聊消息
    connect(home,&Home::sendpixtogroupchat,senddata,&SendData::on_sendpixtogroupchat);//往群聊发送图片
    connect(senddata,&SendData::sendpixingroup,home,&Home::on_sendpixingroup);//收到群聊中的图片
    connect(home,&Home::sendfiletogroup,senddata,&SendData::on_sendfiletogroup);//往群聊中发送文件
    connect(senddata,&SendData::ingroupsendfile,home,&Home::on_ingroupsendfile);//接收群聊发送的文件
    connect(home,&Home::closehome,senddata,&SendData::on_closehome);//关闭程序信号槽
    connect(senddata,&SendData::deletesocket,this,[=](){//关闭窗口，释放资源
        close_num++;
        if(close_num>=2){
            subthread->quit();
            subthread->wait();
            subthread->deleteLater();
            senddata->deleteLater();
        }
    });
    connect(regither,&Register::closewidget,this,[=](){
        regither->hide();
    });

}

Form::~Form()
{
    delete regither;
    delete home;
    delete ui;
}

void Form::init()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);


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
    ui->label->setStyleSheet(
        "border-radius: 50%;"
        "background-color: transparent;"
        );
    QPixmap pixmap(":/icon/3.png");
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
}

void Form::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(240,240,240));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}

void Form::on_pushButton_3_clicked()
{
    this->close();
}


void Form::on_pushButton_clicked()//点击登录
{
    int id=ui->lineEdit->text().toInt();
    QString password=ui->lineEdit_2->text();
    emit userlogon(id,password);
}


void Form::on_pushButton_2_clicked()
{
    regither->show();
}

