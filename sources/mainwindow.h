#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void draw_sea(double height, double range_x); //нарисовать море
    double dydt(double x, double y); //производная функции
    double runge_kutta(double x, double y); //метод рунге-кутта
    void create_vectors_without_resistance(); //заполнение массива данными при случае без учета сопр воды
    void create_vectors_with_resistance(); //с учетом сопр воды
    void output();

    ~MainWindow();

private slots:
   void on_pushButton_clicked();

   void on_radioButton_3_clicked();

   void on_radioButton_4_clicked();

   void on_doubleSpinBox_H_valueChanged(double arg1);

   void on_radioButton_2_clicked();

   void on_radioButton_clicked();

   void on_doubleSpinBox_p0_valueChanged(double arg1);

   void on_doubleSpinBox_V_valueChanged(double arg1);

   void on_doubleSpinBox_p1_valueChanged(double arg1);

   void on_doubleSpinBox_coeff_valueChanged(double arg1);

   void TimerSlot(); //для анимации графика

   void on_pushButton_stop_clicked();

   void on_action_triggered();

   void on_action_4_triggered();

private:
    double lBegin=0; // начало по координате l, начало движения с (0;0)
    double Y=0;
    QVector<double> l,h; //вектора со значениями графика
    QVector<double> x,y; //для анимации рисовки графика
    QVector<double> hH,lH;
    bool end = false; //рисовка графика закончена
    int times = 1; //насколько быстро будет рисоваться график
    int counter = 0;

    Ui::MainWindow *ui;

    //для анимации рисовки графика
    QTimer *timer;
    int time;

    //----------------------------------------
    //все поля для формул вычисления траектории, с которыми можно проводить вычислительные операции
    double v=45; // скорость
    double t = 0; // момент времени
    double step = 0.1; //шаг времени
    int i = 0; //переменная количества точек
    int i_animation = 0; //количество уже построенных точек для анимации
    double g = 9.8; //ускорение свободного падения
    double Height=1000; //на какую высоту подняться.переменные заполнены сначала данными по умолчанию
    double L; // лодка пройдёт расстояние
    double p0=1020;// плотность воды, кг/м^3
    double p1=800;// средняя плотность подлодки
    double coeff=0.04; //коэф. сопротивления воды
    double y_next; //следующий y для методы рунге-кутты
    double h_for_runge_kytta = 0.01; //шаг для метода рунге-кутты
    double k1, k2, k3, k4;


};
#endif // MAINWINDOW_H
