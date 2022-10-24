#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "task_condition.h"
#include "input_data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->doubleSpinBox_coeff->clear();
    ui->doubleSpinBox_coeff->setEnabled(false);

    draw_sea(Height,Height*1.5);

    //некоторые настройки графика
    ui->widget->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
    ui->widget->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    ui->widget->yAxis->setLabel("ось H");
    ui->widget->xAxis->setLabel("ось L");
    ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow); // оси ввиде стрелочек
    ui->statusbar->showMessage("Программа \"Моделирование всплытия подводной лодки\" готова к использованию");
}


void MainWindow::draw_sea(double height, double range_x) //рисуем морюшко
{
    ui->widget->clearGraphs();

    ui->widget->yAxis->setRange(0,height*1.5);
    ui->widget->xAxis->setRange(0,range_x); // диапазон осей

    for (int i = 0; i <= range_x*10; i++)
    {
          hH.push_back(height);
          lH.push_back(i);
     }

    ui->widget->addGraph(); //добавить график на виджет
    ui->widget->graph(0)->addData(lH,hH);//установить графику значения
    ui->widget->graph(0)->setPen(QPen(Qt::white)); // сама линия графика
    ui->widget->graph(0)->setBrush(QBrush(QColor(30, 144, 255, 50)));
    ui->widget->replot(); //нарисовать график

    hH.clear();
    lH.clear();
}

void MainWindow::on_pushButton_clicked() //кнопка "Построить график" нажата
{
    if (ui->doubleSpinBox_H->text() == "" && ui->doubleSpinBox_V->text() == "" && ui->doubleSpinBox_p0->text() == "" && ui->doubleSpinBox_p1->text() == "")
    {
        if (ui->radioButton_2->isChecked() && ui->doubleSpinBox_coeff->text() == "" )
          ui->statusbar->showMessage("Вы ввели не все входные данные, запуск рисовки графика невозможен");
        else ui->statusbar->showMessage("Вы ввели не все входные данные, запуск рисовки графика невозможен");
    }
    else
    {
        //СТРОИМ НОВЫЙ ГРАФИК ! ! !
        ui->statusbar->showMessage("Данные были переданы. График движения подводной лодки рисуется...");

        ui->pushButton->setEnabled(false); // деактивируем кнопку, чтобы всякие шаловливые ручки на неё не тыкали, пока строится график

        ui->widget->clearGraphs();

        draw_sea(Height,Height*1.5);

        if (ui->radioButton->isChecked())
        {
            create_vectors_without_resistance();
        }
        else if (ui->radioButton_2->isChecked())
        {
            create_vectors_with_resistance();
        }

        draw_sea(Height,l[i]*1.5);

        //создание таймера, пересдача сигнала таймеру
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
        timer->start(times);

        output();
    }
}

void MainWindow::output()
{
    //Вывод данных
    ui->output_t->setText(QString::number(t));
    ui->output_L->setText(QString::number(L)+" м");
    ui->output_number_of_points->setText(QString::number(i));
    ui->output_l->setText(QString::number(l[i])+"L");
    ui->output_h->setText(QString::number(h[i])+"H");
}


void MainWindow::TimerSlot()
{
    if (time <= times*i )
    {
       if (i_animation != i+1)
       {
           x.push_back(l[i_animation]);
           y.push_back(h[i_animation]);
        i_animation+=1;
       }
        time += times;
    }
    else
    {
        timer->stop();

        //чтобы одно построение графика не влияло на построение других
        time = 0;
        l.clear();
        h.clear();
        x.clear();
        y.clear();
        i=0;
        i_animation=0;
        t=0;
        ui->pushButton->setEnabled(true);
        ui->statusbar->showMessage("График был успешно изображён. Введите или выберите новые данные и нажмите кнопку \"Построить траекторию всплытия\"");
    }

    ui->widget->addGraph(); //добавить график на виджет
    ui->widget->graph(1)->addData(x,y);//установить графику значения
    ui->widget->graph(1)->setPen(QPen(Qt::black)); // сама линия графика чёрного цвета
    //ui->statusbar->showMessage(QString::number(i));
    ui->widget->replot(); //нарисовать график
}

//----------------------------
//синхронизация переменных с формы с полями класса
//как только значение в окне спинбокса меняется - меняется и переменная

void MainWindow::on_doubleSpinBox_H_valueChanged(double arg1)
{
    Height = arg1;
    draw_sea(Height,Height*1.5);
}

void MainWindow::on_doubleSpinBox_V_valueChanged(double arg1)
{
    v = arg1;
}

void MainWindow::on_doubleSpinBox_p0_valueChanged(double arg1)
{
   p0 = arg1;
   ui->doubleSpinBox_p1->setMaximum(arg1-1); //чтобы значение p1 было всегда меньше p0
}

void MainWindow::on_doubleSpinBox_p1_valueChanged(double arg1)
{
    p1 = arg1;
}

void MainWindow::on_doubleSpinBox_coeff_valueChanged(double arg1)
{
    coeff = arg1;
}
//----------------------------

void MainWindow::on_radioButton_clicked() //если сменится кнопка выбора на "без сопротивления", то выключить окошко с коэффициентом
{
    ui->doubleSpinBox_coeff->clear();
    ui->doubleSpinBox_coeff->setEnabled(false);
}

void MainWindow::on_radioButton_2_clicked()
{
    ui->doubleSpinBox_coeff->setEnabled(true);
    if (ui->radioButton_4->isChecked())
        {
           ui->doubleSpinBox_coeff->setValue(0.04);
        }
}

//----------------------------
//формирование массивов с данными

void MainWindow::create_vectors_without_resistance() //вычисление по формулам без учёта сопротивления воды
{
    //первые значения
     l.push_back(0.0);
     h.push_back(0.0);

     while (h[i] <= Height)
     {
         l.push_back(0);
         h.push_back(0);

         i++;
         t+=step;

         l[i] = v * t;

         h[i] = (g * (p0 - p1) * (pow(l[i], 2))) / (p1 * (pow(v, 2)));
     }

     L = v * t;
}

double MainWindow::dydt(double t, double y)
{
    return (g * (p0 - p1)*t*t) / p1 - coeff *y*t;
}

double MainWindow::runge_kutta(double t, double y)
{
    k1 = h_for_runge_kytta * dydt(t + h_for_runge_kytta / 2, y);
    k2 = h_for_runge_kytta * dydt(t + h_for_runge_kytta / 2, y + k1 / 2);
    k3 = h_for_runge_kytta * dydt(t + h_for_runge_kytta / 2, y + k2 / 2);
    k4 = h_for_runge_kytta * dydt(t + h_for_runge_kytta / 2, y + k3);

    y_next = y + (1.0 / 6.0) * (k1 + 2*k2 + 2*k3 + k4); //новый y вычисляется на основе предыдущего значения и приближения

    return y_next;
}

void MainWindow::create_vectors_with_resistance() //вычисление по формулам с учётом сопротивления воды
{
    l.push_back(0);
    h.push_back(0);

     while (h[i] <= Height)
        {
            double y;
            l.push_back(0);
            h.push_back(0);
            i++;
            t += step;
            y = runge_kutta(t, h[i - 1]);

            l[i] = v * t;
            h[i] = y;
        }

     L = v * t;
}

//----------------------------

void MainWindow::on_pushButton_stop_clicked() //Очистка графика
{
    timer->stop();

    ui->widget->clearGraphs();

    draw_sea(Height,Height*1.5);

    //чтобы одно построение графика не влияло на построение других
    time = 0;
    l.clear();
    h.clear();
    x.clear();
    y.clear();
    i=0;
    i_animation=0;
    t=0;
    ui->pushButton->setEnabled(true);
    ui->statusbar->showMessage("График был очищен. Введите или выберите новые данные и нажмите кнопку \"Построить траекторию всплытия\"");
}


//----------------------------
//установка данных по умолчанию
void MainWindow::on_radioButton_3_clicked()
{
     ui->doubleSpinBox_H->clear();
     ui->doubleSpinBox_V->clear();
     ui->doubleSpinBox_p0->clear();
     ui->doubleSpinBox_p1->clear();
     ui->doubleSpinBox_coeff->clear();

}

void MainWindow::on_radioButton_4_clicked()
{
    ui->doubleSpinBox_H->clear();
    ui->doubleSpinBox_V->clear();
    ui->doubleSpinBox_p0->clear();
    ui->doubleSpinBox_p1->clear();
    ui->doubleSpinBox_coeff->clear();

    if (ui->radioButton->isChecked())
    {
       ui->doubleSpinBox_H->setValue(1000);
       ui->doubleSpinBox_V->setValue(45);
       ui->doubleSpinBox_p0->setValue(1024);
       ui->doubleSpinBox_p1->setValue(800);
    }
    else if (ui->radioButton_2->isChecked())
    {
       ui->doubleSpinBox_H->setValue(1000);
       ui->doubleSpinBox_V->setValue(45);
       ui->doubleSpinBox_p0->setValue(1024);
       ui->doubleSpinBox_p1->setValue(800);
       ui->doubleSpinBox_coeff->setEnabled(true);
       ui->doubleSpinBox_coeff->setValue(0.04);
    }

}
//----------------------------

MainWindow::~MainWindow()
{
    delete ui;
}


//Создаём НЕМОДАЛЬНЫЕ окна
void MainWindow::on_action_triggered()
{
    task_condition* window = new task_condition(this);
    window->setWindowTitle("Постановка физической задачи");
    window->show();
}

void MainWindow::on_action_4_triggered()
{
    input_data* window_2 = new input_data(this);
    window_2->setWindowTitle("О входных данных");
    window_2->show();
}
