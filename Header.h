#ifndef HEADER_H
#define HEADER_H

#endif // HEADER_H

#include <QApplication>
#include <QTextStream>
#include <QtMath>
#include <QFile>
#include <cmath>
#include <QVector>
#include <QString>

void Inv(QVector<QVector<double>> matrix);

///// ///// ///// ///// //// ///
class Least_Squares_Method
{

    QVector<QVector<double>> Transpose(QVector<QVector<double>> matrix);
    //============================================
    // умножение марицы на матрицу или вектор
    QVector<double> Multiply(QVector<QVector<double>> matrix, QVector<double> Vector);
    QVector<QVector<double>> Multiply(QVector<QVector<double>> matrix_A, QVector<QVector<double>> matrix_B);
    //==============================================================

    QVector<QVector<double>> Inverse_Matrix(QVector<QVector<double>> matrix);

 public:
    QVector<QVector<double>> X;// двумерный массив признаков
    QVector<double> Y; // одномерный массив значений
    QVector<double> Sigma; // массив оценок

    //=======метрики обучения========
    double RMSE; // среднеквадратичное отклонение (извлекается корень)
    double MAE; // средняя абсолютная ошибка
    //===============================
    void Calculate_Metrics(const QVector<double> & Y, const QVector<double> &Predicted); // расчет метрик
    QVector<double> Create_Predicted_Set (const QVector<QVector<double>> & X);

    //===============================
    void Calculate();
    double predict(QVector<double>& features);
};


class DataSet
{
public:
    bool correct;
    bool readData(QString& File_Path, const QString& separator = "|");
    QVector<QVector<double>> X;
    QVector<double> Y;

    QString File_Path;


};


