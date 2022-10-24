#include "task_condition.h"
#include "ui_task_condition.h"

task_condition::task_condition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::task_condition)
{
    ui->setupUi(this);
}

task_condition::~task_condition()
{
    delete ui;
}
