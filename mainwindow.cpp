#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // Создаем виджет для построения графиков

    ui->setupUi(this);
    // Начальные настройки интерфейса

    // доступна только кнопка выбора файла с входными данными, остальные недоступны
    ui->Calculate_Button->setEnabled(false);
    ui->Predict_Label->setVisible(false);
    ui->Input_Params->setVisible(false);
    ui->Plot_Button->setVisible(false);
    ui-> Input_Params-> setVisible(false);
    ui->Reset_Button->setEnabled(false);
    ui->Predict_Button->setVisible(false);
    ui->Predict_Label2->setVisible(false);
    ui->MSE_label->setVisible(false);
    ui->MAE_label->setVisible(false);

    ui->Plot_Widget->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_Select_File_Button_clicked()
{// получаем путь к файлу
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл с данными");
    if (fileName.isEmpty()) return;

     set.correct =  set.readData(fileName); // чтение данных из файла с проверкой на успешность

    // Чтение данных из файла

    if (set.correct == false)
    {
         QMessageBox::warning(this, "Ошибка", QString("Файл пуст или поврежден!"));
        return;//
    }

    ui->File_Name_Out->setText(fileName);
    // После загрузки активируем кнопку расчета и сброса, запрещаем загрузку данных
    ui->Calculate_Button->setEnabled(true);
    ui->Reset_Button->setEnabled(true);
    ui->Select_File_Button->setEnabled(false);
}


void MainWindow::on_Calculate_Button_clicked()
{// если нажата кнопка расчета, запускаем вычисления

    model.X = set.X; model.Y=set.Y;


    int Number_of_Features =  set.X[0].size(); // количество признаков - это число столбцов
    model.Calculate(); // вполнение расчета коэффициентов
    model.Calculate_Metrics(model.Y, model.Create_Predicted_Set(model.X)); // расчет метрик обучения

    ui->MSE_label->setVisible(true);
    ui->MAE_label->setVisible(true);

    ui->MAE_label->setText(QString("Средняя абсолютная ошибка: %1").arg(model.MAE, 0, 'f', 4));
    ui->MSE_label->setText(QString("Среднеквадратическое отклонение : %1").arg(model.RMSE, 0, 'f', 4));



    // Активируем предсказание
    ui->Predict_Button->setVisible(true); // открывем кнопку предсказаний
    ui->Predict_Label->setVisible(true); // уведомляем о доступе
    ui->Input_Params->setVisible(true); // открываем окно ввода

    ui->Predict_Label->setText(
      QString("Введите %1 параметра(ов) через пробел:").arg(Number_of_Features));

    // при одном признаке пересоздаем массив Х
    if (Number_of_Features == 1)
    {
        QVector <double> X;
        for (int i = 0; i < model.X.size(); i++)
        {
            X.append(model.X[i][0]);
        }
        Plot(ui->Plot_Widget, X);

    }
}


void MainWindow::on_Reset_Button_clicked()
{// нажата кнопка

    model.X.clear(); // уничтожние старых данных
    model.Y.clear();
    set.X.clear();
    set.Y.clear();
   // set.Sigma.clear();
    // возвращение окна к режиму по-умолчанию
    ui->Calculate_Button->setEnabled(false);

    ui->Predict_Label->setVisible(false);
    ui->Input_Params->setVisible(false);
    ui->Plot_Button->setVisible(false);
    ui-> Input_Params-> setVisible(false);
    ui->Reset_Button->setEnabled(false);
    ui->Predict_Button->setVisible(false);
    ui->Predict_Label2->setVisible(false);
    ui->Select_File_Button->setEnabled(true);
    ui->MSE_label->setVisible(false);
    ui->MAE_label->setVisible(false);


    ui->Plot_Widget->clearGraphs();  // Удаляем все графики
    ui->Plot_Widget->clearItems();   // Удаляем тексты, линии и другие элементы
    ui->Plot_Widget->xAxis->setRange(0, 10);  // Возвращаем начальный масштаб X
    ui->Plot_Widget->yAxis->setRange(0, 10);  // Возвращаем начальный масштаб Y
    ui->Plot_Widget->replot();      // Обновляем
}


void MainWindow::on_Predict_Button_clicked()
{ //кнопка предсказания нажата
    // Получение параметров для предсказания (вводятся через пробел)
    QStringList params = ui->Input_Params->text().split(" ", QString::SkipEmptyParts);

    if (params.size() != set.X[0].size())
    { // если параметров невероне количество
        QMessageBox::warning(this, "Ошибка",
                             QString("Требуется %1 параметров").arg(set.X[0].size()));
        return;
    }

    QVector<double> Predict_Data; // вектор с написанными параметрами
    for (const QString& param : params)
    { // цикл по количеству чисел (которые параметры), разделенные через сепаратор
        bool ok; // удачное преобразование в число
        double value = param.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "Ошибка", "Некорректные параметры");
            return; // при неудаче выдать ошибку и вернуться
        }
        Predict_Data.append(value);
    }

    double prediction = model.predict(Predict_Data); // вызов метода предсказания и вывод его под строкой ввода
    ui->Predict_Label->setText(QString("Предсказанное значение: %1").arg(prediction));

    // Обновляем график для одного признака
    int Numbers_of_Features = set.X[0].size();
    if (Numbers_of_Features == 1)
    {
        double predict_value = Predict_Data[0];
        QVector <double> X;
        for (int i = 0; i < model.X.size(); i++)
        {
            X.append(model.X[i][0]);
        }
        //model.X.append(predict_value); // добавляем в конец поисковой значение
        Continue_Plot(ui->Plot_Widget, X, Predict_Data[0], prediction);
    }
}



void MainWindow::Plot(QCustomPlot *customPlot, QVector<double> x)
{

    QVector<double> Predicted_Values;
    QVector <double> Feature;

    Feature.append(model.X[0][0]);

    for (int i = 0; i < model.Y.size(); i++)
    {
        if (i != 0)
        {
            Feature[0] = model.X[i][0];
        }
        Predicted_Values.append(model.predict(Feature));
    }

    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue)); // синий график

    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5)); // точки
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone); // нет связи между точками

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red)); // красный график

    // создаем оси
    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);
    // связка графиков и диапазонов
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // выдаем данные графикам
    customPlot->graph(0)->setData(x, model.Y);
    customPlot->graph(1)->setData(x, Predicted_Values);
    // масштабироваие
    customPlot->graph(0)->rescaleAxes();

    customPlot->graph(1)->rescaleAxes(true);
    // просмотр с момощью мыши
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->Plot_Widget->replot();
}






void MainWindow::Continue_Plot(QCustomPlot *customPlot, QVector<double> x, double X, double Y)
{ // дополнительные параметры - это координаты точки, нужной для иллюстрации

    QVector<double> Predicted_Values;
    QVector <double> Feature;

    Feature.append(model.X[0][0]);

    for (int i = 0; i < model.Y.size(); i++)
    {
        if (i != 0)
        {
            Feature[0] = model.X[i][0];
        }
        Predicted_Values.append(model.predict(Feature));
    }
    x.append(X); // добавляем координаты точек
    Predicted_Values.append(Y);


    // аналогично функции Plot
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));

    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);

    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::red));

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    customPlot->yAxis2->setVisible(true);
    customPlot->yAxis2->setTickLabels(false);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    customPlot->graph(0)->setData(x, model.Y);
    customPlot->graph(1)->setData(x, Predicted_Values);

    customPlot->graph(0)->rescaleAxes();

    customPlot->graph(1)->rescaleAxes(true);

    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // Создаем отдельный график для точки предсказания
    customPlot->addGraph();
    customPlot->graph(2)->setPen(QPen(QColor(148, 0, 211))); // Фиолетовый цвет
    customPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10)); // Большая точка

    // Создаем данные для точки (один элемент)
    QVector<double> predX(1), predY(1);
    predX[0] = X; // X-координата
    predY[0] = Y;          // Y-координата (предсказанное значение)

    customPlot->graph(2)->setData(predX, predY);
    ui->Plot_Widget->replot();
}














