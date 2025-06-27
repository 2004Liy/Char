#include "prompt.h"
#include "ui_prompt.h"

Prompt::Prompt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Prompt)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   border-radius: 15px;"
        "   border: 2px solid #CCCCCC;"
        "}"
        );
}

Prompt::~Prompt()
{
    delete ui;
}
