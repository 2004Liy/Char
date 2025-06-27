#include "showgroupmember.h"
#include "ui_showgroupmember.h"
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPainter>
#include<QWidget>
#include<QPushButton>
#include<QLabel>

ShowGroupmember::ShowGroupmember(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ShowGroupmember)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout *layout=new QVBoxLayout(ui->widget);
    layout->setSpacing(0);
    layout->addStretch();
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ShowGroupmember::~ShowGroupmember()
{
    delete ui;
}

void ShowGroupmember::addbtn()
{
    QWidget *widget=new QWidget(this);
    QHBoxLayout *layout=new QHBoxLayout(widget);
    layout->setSpacing(15);
    QPushButton *btn=new QPushButton(widget);
    connect(btn,&QPushButton::clicked,[=](){
        emit addnewmember();
    });
    btn->setStyleSheet(
        "background-color: rgba(255, 255, 255, 0);"
        );
    btn->setMaximumSize(QSize(35,35));
    btn->setMinimumSize(QSize(35,35));
    btn->setIconSize(QSize(30,30));
    btn->setIcon(QPixmap(":/icon/45.png"));
    layout->addWidget(btn);
    QLabel *label=new QLabel(widget);
    label->setText("邀请新成员");
    layout->addWidget(label);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->widget->layout());
    mainLayout->insertWidget(mainLayout->count()-1,widget);
}

void ShowGroupmember::additem(QString name, QPixmap pix)
{
    QWidget *widget=new QWidget(this);
    QHBoxLayout *layout=new QHBoxLayout(widget);
    layout->setSpacing(15);
    QLabel *label1=new QLabel(widget);
    label1->setMaximumSize(QSize(35,35));
    label1->setMinimumSize(QSize(35,35));
    QPixmap pixmap = pix.scaled(label1->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);  // 缩放图片以适应标签大小
    QPixmap circularPixmap(label1->size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, label1->width(), label1->height());
    label1->setPixmap(circularPixmap);
    label1->setAlignment(Qt::AlignCenter);
    layout->addWidget(label1);
    QLabel *label2=new QLabel(widget);
    label2->setText(name);
    layout->addWidget(label2);
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->widget->layout());
    mainLayout->insertWidget(mainLayout->count()-1,widget);
}

void ShowGroupmember::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    emit closewidget();
    this->close();
}

void ShowGroupmember::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}


