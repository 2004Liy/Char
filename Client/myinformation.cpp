#include "myinformation.h"
#include "ui_myinformation.h"
#include<QPainter>
#include<QFileDialog>

Myinformation::Myinformation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Myinformation)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

Myinformation::~Myinformation()
{
    delete ui;
}

void Myinformation::init(int id, QString name, QPixmap pix)
{
    ui->label_5->setText(QString::number(id));
    ui->lineEdit_2->setText(name);
    QPixmap pixmap = pix.scaled(ui->pushButton->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    QPixmap circularPixmap(ui->pushButton->size());
    circularPixmap.fill(Qt::transparent);
    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(pixmap));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, ui->pushButton->width(), ui->pushButton->height());
    ui->pushButton->setIcon(circularPixmap);
}

void Myinformation::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 15, 15);
}

void Myinformation::on_pushButton_2_clicked()
{
    emit confirmclicked(ui->lineEdit_2->text());
}


void Myinformation::on_pushButton_3_clicked()
{
    emit cancelclicked();
}


void Myinformation::on_pushButton_clicked()
{
    QString path=QFileDialog::getOpenFileName(this,"选择头像","",tr("PNG图片 (*.png)"));
    if(path.isEmpty()){
        return;
    }
    QByteArray newicon;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    const qint64 CHUNK_SIZE = 1024 * 50;
    while(!file.atEnd()){
        QByteArray chunk=file.read(CHUNK_SIZE);
        newicon.append(chunk);
    }
    file.close();
    QPixmap pix;
    pix.loadFromData(newicon);
    emit iconchanged(path);
    emit tohomeicon(newicon);
    init(ui->label_5->text().toInt(),ui->lineEdit_2->text(),pix);
}

