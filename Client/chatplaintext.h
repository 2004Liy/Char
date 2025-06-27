#ifndef CHATPLAINTEXT_H
#define CHATPLAINTEXT_H

#include <QWidget>
#include <QMouseEvent>
#include <QApplication>

namespace Ui {
class ChatPlainText;
}

class ChatPlainText : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPlainText(QWidget *parent = nullptr);
    ~ChatPlainText();
    bool havebtn=0;
    void addbutton();
    void delbutton();

private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_sticker(QPixmap pix,QString path);



private:
    QHash<QString, QString> m_imageIdToPath;
    Ui::ChatPlainText *ui;


public:signals:
    void sendmsg(QString msg);
    void sendfile();
    void sendpicture();
    void addnewmember();
};

#endif // CHATPLAINTEXT_H
