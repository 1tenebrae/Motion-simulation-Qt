#include "input_data.h"
#include "ui_input_data.h"

input_data::input_data(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::input_data)
{
    ui->setupUi(this);
}

input_data::~input_data()
{
    delete ui;
}
