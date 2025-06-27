#ifndef STICKERWIDGET_H
#define STICKERWIDGET_H

#include <QWidget>

namespace Ui {
class Stickerwidget;
}

class Stickerwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Stickerwidget(QWidget *parent = nullptr);
    ~Stickerwidget();

protected:
    void leaveEvent(QEvent *event) override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::Stickerwidget *ui;

public:signals:
    void focusOut();
    void sticker1();
    void sticker2();
    void sticker3();
    void sticker4();
    void sticker5();
    void sticker6();
};

#endif // STICKERWIDGET_H
