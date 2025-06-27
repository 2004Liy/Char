#ifndef MYINFORMATION_H
#define MYINFORMATION_H

#include <QWidget>

namespace Ui {
class Myinformation;
}

class Myinformation : public QWidget
{
    Q_OBJECT

public:
    explicit Myinformation(QWidget *parent = nullptr);
    ~Myinformation();

public:

    void init(int id,QString name,QPixmap pix);

protected:
    void paintEvent(QPaintEvent* event)override;

private:
    Ui::Myinformation *ui;
public:signals:
    void confirmclicked(QString name);
    void cancelclicked();
    void iconchanged(QString path);
    void tohomeicon(QByteArray data);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
};

#endif // MYINFORMATION_H
