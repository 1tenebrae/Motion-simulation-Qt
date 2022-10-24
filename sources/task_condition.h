#ifndef TASK_CONDITION_H
#define TASK_CONDITION_H

#include <QDialog>

namespace Ui {
class task_condition;
}

class task_condition : public QDialog
{
    Q_OBJECT

public:
    explicit task_condition(QWidget *parent = nullptr);
    ~task_condition();

private:
    Ui::task_condition *ui;
};

#endif // TASK_CONDITION_H
