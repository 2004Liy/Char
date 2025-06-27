#ifndef MYSENDFILE_H
#define MYSENDFILE_H

#include <QWidget>

namespace Ui {
class MySendfile;
}

class MySendfile : public QWidget
{
    Q_OBJECT

public:
    explicit MySendfile(QWidget *parent = nullptr);
    ~MySendfile();

    void setfileicon(QString filepath);
    void setvalue(int value);

private:
    Ui::MySendfile *ui;
};

#endif // MYSENDFILE_H
