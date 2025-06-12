#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include"Header.h"
#include "qcustomplot.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Plot(QCustomPlot *customPlot,  QVector<double> x);
    void Continue_Plot(QCustomPlot *customPlot, QVector<double> x, double X, double Y);
private slots:


    void on_Select_File_Button_clicked();

    void on_Calculate_Button_clicked();

   // void on_Input_Params_editingFinished();

    void on_Reset_Button_clicked();

    void on_Predict_Button_clicked();

private:
    Ui::MainWindow *ui;
    Least_Squares_Method model;
    DataSet set;
    QCustomPlot *customPlot;
};











#endif // MAINWINDOW_H
