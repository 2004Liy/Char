#include "addgroupitem.h"
#include "ui_addgroupitem.h"
#include<QPainter>

Addgroupitem::Addgroupitem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Addgroupitem)
{
    ui->setupUi(this);
    ui->radioButton->setStyleSheet(
        "QRadioButton::indicator {"
        "    width: 20px;"
        "    height: 20px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "    image: url(:/icon/43.png);"
        "}"
        "QRadioButton::indicator:checked {"
        "    image: url(:/icon/42.png);"
        "}"
        );
}

Addgroupitem::~Addgroupitem()
{
    delete ui;
}

void Addgroupitem::init(QString name, QPixmap pixmap, int friendid)
{
    ui->label_2->setText(name);
    id=friendid;
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

int Addgroupitem::getid()
{
    return id;
}

bool Addgroupitem::ischecked()
{
    return ui->radioButton->isChecked();
}

void Addgroupitem::membermark()
{
    ui->radioButton->setChecked(1);
    ui->radioButton->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    ui->radioButton->setFocusPolicy(Qt::NoFocus);
    ismember=1;
}

bool Addgroupitem::member()
{
    return ismember;
}
