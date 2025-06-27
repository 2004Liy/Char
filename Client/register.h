#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>


namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:

protected:
    void paintEvent(QPaintEvent *event)override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

public:signals:
    void closewidget();
    void login(QString name,QString password);

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
