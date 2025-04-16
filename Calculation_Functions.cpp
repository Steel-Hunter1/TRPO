
#include "Header.h"

void Approx_Function(double*X, double*Y, double&k, double&b)
// на вход подается два массива, со значениями X и Y
{ // линейная функция y = kx+b
    int Size = sizeof(X);
    //double k = 0;
   // double b = 0;
    double SummXY=0;// сумма Xi * Yi
    for (int i = 0; i < Size; i++)
    {
        SummXY+= X[i]*Y[i];
    }
    
    double SummX=0;// сумма Xi
    for (int i = 0; i < Size; i++)
    {
        SummX += X[i];
    }

    double SummY=0;// Сумма Yi
    for (int i = 0; i < Size; i++)
    {
        SummY+=Y[i];
    }
    double Summ_SquareX = 0;// Сумма (X^2)i
    for (int i = 0; i < Size; i++)
    {
        Summ_SquareX+=X[i]*X[i];
    }

    // k = (n*(сумма Xi * Yi) - (сумма Xi)*(Сумма Yi))/(n*(Сумма (X^2)i) - (сумма Xi)^2)
    k = (Size*SummXY - SummX*SummY)/(Size*Summ_SquareX - SummX*SummX);
    // b = ((Сумма Yi)-k*(сумма Xi))/n
    b = (SummY-k*SummX)/Size;
}

void Least_Squares_Method::Calculate()
{
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

double Least_Squares_Method::Determinant (QVector<QVector<double>> matrix)
{// нахождение определителя матрицы
    int N = matrix.size();
    int M =  matrix[0].size();
    double Det = 1; // определитель
    if ((N==1)and(M==1))
    {
        return matrix[0][0];
    }
    if ((N==2)and(M==2))
    {
        Det =  matrix[0][0] *  matrix[1][1] - ( matrix[1][0] *  matrix[0][1]);
    }

    for (int i = 0; i < N; i++) // цикл по столбцам
    {
        int Max_Row = i; // индекс строки, где в i-ом столбце элемент максимален по модулю
        for (int j = i+1; j < N; j++) // цикл, который перебирает строки начиная от i+1
        {
            if (abs(matrix[j][i]) > abs(matrix[j][Max_Row]))// если находится элемент, который
            {
                Max_Row = j;
            }
        }
        if (Max_Row != i)// если строка с максимальным элементом не совпадают, надо поменять строки местами
        {// чтобы наибольший элемент занял место на главной диагонали
            matrix[i].swap(matrix[Max_Row]);
            Det*= -1; // при замене строк меняем на противоположенный знак
        }
        for (int j = i+1; j < N; j++) // ниже максимального элемента нужно обнулить
        {
            double Coeff = matrix[j][i] / matrix [i][i];
            for (int k = i; k < N; k++)// обнуление
            {
                matrix[j][k] -= Coeff*matrix[i][k];

            }
        }
    }
    for (int i =0; i < N; i ++)
    {
        Det*=matrix[i][i];
    }
    return Det;

}

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
    double prediction = Sigma[0]; // intercept (b0)

    for (int i = 0; i < Features.size(); ++i)
    {
        prediction += Sigma[i+1] * Features[i];
    }

    return prediction;
}














