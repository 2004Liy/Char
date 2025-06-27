#ifndef PROMPT_H
#define PROMPT_H

#include <QWidget>

namespace Ui {
class Prompt;
}

class Prompt : public QWidget
{
    Q_OBJECT

public:
    explicit Prompt(QWidget *parent = nullptr);
    ~Prompt();

private:
    Ui::Prompt *ui;
};

#endif // PROMPT_H
