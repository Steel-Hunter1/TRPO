
#include "Header.h"

void Least_Squares_Method::Calculate_Metrics(const QVector<double> & Y, const QVector<double> & Predicted)
{ // расчет метрик обучения

    double sum = 0.0;
    int n = Y.size();

    for (int i = 0; i < n; i++)
    {
        sum += qAbs(Y[i] - Predicted[i]);
    }
    this->MAE = sum/n; // рассчет средней абсолютой ошибки

    sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        double error = Y[i] - Predicted[i];
        sum += error * error;
    }
    this->RMSE = sqrt(sum/n); // рассчет средеквадратичного отклонения
}

void Least_Squares_Method::Calculate()
{ // выполнение расчета
    if (X.size() == 0)
    {
        return;
    }
    int N = X.size();
    int M = X[0].size();
    // согласно формуле (сигма = (  (X)^T * (X)  )^(-1) * (X)^T * Y
    QVector<QVector<double>> X_Extended(N, QVector<double>(M + 1));
    // на один столбец больше
    for (int i = 0; i < N; ++i)
    {
        // Первый столбец - единицы для расчета свободного члена (константы)
        X_Extended[i][0] = 1.0;

        // Копируем исходные признаки
        for (int j = 0; j < M; ++j)
        {
            X_Extended[i][j+1] = X[i][j]; // Сдвиг на 1 из-за добавленного столбца
        }
    }
    QVector<QVector<double>> Transposed_X = Transpose(X_Extended); // транспонируем X
    QVector<QVector<double>> X_multip_Xt = Multiply (Transposed_X, X_Extended); // умножение транспонированной X на обычный X
    QVector<QVector<double>> Inversed_Res = Inverse_Matrix( X_multip_Xt); // инвертирование матрицы с предыд. шага
    QVector<QVector<double>> X_Res = Multiply(Inversed_Res, Transposed_X);
    this->Sigma  = Multiply(X_Res, this->Y); // итоговый результат
}

QVector<QVector<double>> Least_Squares_Method::Transpose(QVector<QVector<double>> matrix)
{// транспонирование матрицы

    int rows = matrix.size(); // количество строк
    int cols = matrix[0].size(); // количество столбцов

    // Создаём транспонированную матрицу с обратными размерами
    QVector<QVector<double>> result(cols, QVector<double>(rows));

    for (int i = 0; i < rows; ++i)
    {
        // Проверка на согласованность размеров
        if (matrix[i].size() != cols)
        {
            // qWarning() << "Несогласованная матрица в строке" << i;
            return QVector<QVector<double>>();
        }

        for (int j = 0; j < cols; ++j)
        {
            result[j][i] = matrix[i][j];  // Меняем местами индексы
        }
    }

    return result;
}

    /*
    int N = matrix.size();
    int M = matrix[0].size();
    QVector<QVector<double>> Transposed_Matrix(M, QVector<double>(N));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            Transposed_Matrix[j][i] = matrix[i][j];
        }
    }

    return Transposed_Matrix;
}*/
//============================================
// умножение марицы на матрицу или вектор
QVector<double> Least_Squares_Method::Multiply(QVector<QVector<double>> matrix, QVector<double> Vector)
{//
    int n = matrix.size();
    int m = matrix[0].size();
    QVector<double> Multiplied_Matrix(n, 0.0);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            Multiplied_Matrix[i] += matrix[i][j] * Vector[j];
        }
    }

    return Multiplied_Matrix;
}
QVector<QVector<double>> Least_Squares_Method::Multiply(QVector<QVector<double>> matrix_A, QVector<QVector<double>> matrix_B)
{
    int N = matrix_A.size(); // количество строк первой матрицы
    int M =  matrix_B[0].size(); // количество столбцов второй матрицы
    int P =  matrix_A[0].size(); //
    QVector<QVector<double>> Multiplied_Matrix(N, QVector<double>(M, 0.0));

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            for (int k = 0; k < P; ++k)
            {
                Multiplied_Matrix[i][j] +=  matrix_A[i][k] * matrix_B[k][j];
            }
        }
    }

    return Multiplied_Matrix;
}
//==============================================================



QVector<QVector<double>> Least_Squares_Method::Inverse_Matrix(QVector<QVector<double>> matrix)
{// нахождение обратной матрицы
    int N = matrix.size();
    QVector<QVector<double>> Inversed_Matrix (N, QVector<double> (N)); // обратная матрица, которая дописывается справа



    // Создаем единичную матрицу
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == j)
                Inversed_Matrix[i][j] = 1;
            else Inversed_Matrix[i][j]=0;
        }
    }

    // Прямой ход метода Гаусса
    for (int i = 0; i < N; i++)
    {
        // Поиск ведущего элемента в текущем столбце
        int Max_Row = i;
        for (int j = i + 1; j < N; j++)
        {
            if (abs(matrix[j][i]) > abs(matrix[Max_Row][i]))
            {
                Max_Row = j;

            }
        }

        // Перестановка строк
        if (Max_Row != i)
        {
            matrix[i].swap(matrix[Max_Row]);
            Inversed_Matrix[i].swap(Inversed_Matrix[Max_Row]);
        }


        // Нормировка текущей строки
        double pivot = matrix[i][i];
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] /= pivot;
            Inversed_Matrix[i][j] /= pivot;
        }

        // Обнуление столбца под текущим элементом
        for (int j = i + 1; j < N; j++)
        {
            double coeff = matrix[j][i];
            for (int k = 0; k < N; k++)
            {
                matrix[j][k] -= coeff * matrix[i][k];
                Inversed_Matrix[j][k] -= coeff * Inversed_Matrix[i][k];
            }
        }
    }

    // Обратный ход метода Гаусса
    for (int i = N - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            double coeff = matrix[j][i];
            for (int k = 0; k < N; k++)
            {
                matrix[j][k] -= coeff * matrix[i][k];
                Inversed_Matrix[j][k] -= coeff * Inversed_Matrix[i][k];
            }
        }
    }

    return Inversed_Matrix;


}


double  Least_Squares_Method::predict(QVector<double>& Features)
{
    // Расчет предсказания: y = b0 + b1*x1 + b2*x2 + ... + bn*xn
    double prediction = Sigma[0]; // учет b0

    for (int i = 0; i < Features.size(); ++i)
    {
        prediction += Sigma[i+1] * Features[i];
    }

    return prediction;
}

QVector<double> Least_Squares_Method::Create_Predicted_Set(const QVector<QVector<double>> & X)
{ // создает мааси предсказанных значений относительно существующих значений признаков для расчета метрик
    QVector<double> Predicted_Values;
    QVector<double> Features_to_Predict;

    for (int i = 0; i < X.size(); i++) //  количество столбцов
    {
        for (int j = 0; j < X[0].size(); j++) // количество строк
        {
            Features_to_Predict.append(X[i][j]);
        }
    Predicted_Values.append(predict(Features_to_Predict));
    Features_to_Predict.clear();
    }
    return Predicted_Values;
}










