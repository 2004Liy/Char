#include "chatplaintext.h"
#include "ui_chatplaintext.h"
#include<QFileInfo>
#include "stickerwidget.h"
#include <QApplication>
#include <QScreen>
#include<QTextFrame>
#include<QPushButton>
#include<QLayout>

ChatPlainText::ChatPlainText(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatPlainText)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->pushButton_4->setStyleSheet(
        "QPushButton{"
        "background-color: rgb(85, 170, 255);"
        "color: rgb(255, 255, 255);"
        "border-radius:4px;}"
        "QPushButton:hover {"
        "background-color: rgb(50, 131, 231);"
        "color: rgb(255, 255, 255);"
        "border-radius:4px;"
        "}"
        );
    ui->widget->setStyleSheet(
        "border: 0px;"
        "border-top: 1px dotted #bebebe;"
        );
}

ChatPlainText::~ChatPlainText()
{
    delete ui;
}

void ChatPlainText::addbutton()
{
    QPushButton *button=new QPushButton;
    connect(button,&QPushButton::clicked,this,[=](){
        emit addnewmember();
    });
    button->setStyleSheet(
        "background-color: rgba(255, 255, 255, 0);"
        );
    button->setMaximumSize(QSize(30,30));
    button->setMinimumSize(QSize(30,30));
    button->setIconSize(QSize(25,25));
    button->setIcon(QPixmap(":/icon/44.png"));
    QLayout *layout=ui->widget->layout();
    layout->addWidget(button);
    havebtn=1;
}

void ChatPlainText::delbutton()
{
    QLayout *layout=ui->widget->layout();
    QLayoutItem *item = layout->itemAt(layout->count() - 1);
    if (item->widget()) {
        QWidget *button = item->widget();
        layout->removeWidget(button);
        button->hide();
        button->deleteLater();
    }
    havebtn=0;
}

void ChatPlainText::on_pushButton_4_clicked()
{
    QTextDocument* document = ui->textEdit->document();
    QTextFrame* rootFrame = document->rootFrame();
    QTextFrame::iterator it;
    for (it = rootFrame->begin(); !(it.atEnd()); ++it) {
        QTextBlock block = it.currentBlock();
        if (!block.isValid()) continue;
        QTextBlock::iterator fragmentIt;
        for (fragmentIt = block.begin(); !(fragmentIt.atEnd()); ++fragmentIt) {
            QTextFragment fragment = fragmentIt.fragment();
            if (fragment.charFormat().isImageFormat()) {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                QString imageName = imageFormat.property(QTextFormat::UserProperty).toString();
                QString emoji;
                if(imageName==":/icon/35.png"){
                    emoji="😎";
                }else if(imageName==":/icon/36.png"){
                    emoji="😙";
                }else if(imageName==":/icon/37.png"){
                    emoji="😭";
                }else if(imageName==":/icon/38.png"){
                    emoji="😁";
                }else if(imageName==":/icon/41.png"){
                    emoji="😟";
                }else if(imageName==":/icon/40.png"){
                    emoji="🤬";
                }
                QTextCursor cursor = ui->textEdit->textCursor();
                cursor.setPosition(fragment.position());
                cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                cursor.insertText(emoji);
            }
        }
    }
    QString msg=ui->textEdit->toPlainText();
    qDebug()<<"msg:"<<msg<<'\n';
    emit sendmsg(msg);
    ui->textEdit->clear();
}


void ChatPlainText::on_pushButton_2_clicked()
{
    emit sendfile();
}


void ChatPlainText::on_pushButton_3_clicked()
{
    emit sendpicture();
}


void ChatPlainText::on_pushButton_clicked()
{
    Stickerwidget *sticker=new Stickerwidget;
    connect(sticker,&Stickerwidget::sticker1,this,[=](){
        on_sticker(QPixmap(":/icon/35.png"),":/icon/35.png");
    });
    connect(sticker,&Stickerwidget::sticker2,this,[=](){
        on_sticker(QPixmap(":/icon/36.png"),":/icon/36.png");
    });
    connect(sticker,&Stickerwidget::sticker3,this,[=](){
        on_sticker(QPixmap(":/icon/37.png"),":/icon/37.png");
    });
    connect(sticker,&Stickerwidget::sticker4,this,[=](){
        on_sticker(QPixmap(":/icon/38.png"),":/icon/38.png");
    });
    connect(sticker,&Stickerwidget::sticker5,this,[=](){
        on_sticker(QPixmap(":/icon/41.png"),":/icon/41.png");
    });
    connect(sticker,&Stickerwidget::sticker6,this,[=](){
        on_sticker(QPixmap(":/icon/40.png"),":/icon/40.png");
    });
    connect(sticker,&Stickerwidget::focusOut,this,[=](){
        sticker->deleteLater();
    });
    QPoint globalPos = ui->pushButton->mapToGlobal(QPoint(0,0));
    int x = globalPos.x() + (ui->pushButton->width() - sticker->width()) / 2;
    int y = globalPos.y() - sticker->height() - 5;
    sticker->move(x, y);
    sticker->show();
    sticker->setFocus();
}

void ChatPlainText::on_sticker(QPixmap pix,QString path)
{
    QImage image = pix.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation).toImage();
    QString resourceName = QFileInfo(path).fileName();
    ui->textEdit->document()->addResource(QTextDocument::ImageResource, QUrl(resourceName), image);
    QTextImageFormat imgFormat;
    imgFormat.setName(resourceName);
    imgFormat.setWidth(15);
    imgFormat.setHeight(15);
    imgFormat.setProperty(QTextFormat::UserProperty, path);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.insertImage(imgFormat);
}


