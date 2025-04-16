#include "Header.h"

bool DataSet::readData(QString& File_Path, const QString& separator)
{// считывание данных из файла

    QFile file(File_Path);
    //<QVector<double>> X;

   // QVector<double> Y;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    { // проверка на открытие
        return false;
    }
   // if (file.pos() == 0)
     //   return false;
    QTextStream input(&file);

    while (!input.atEnd())
    { // построчное считывание файла
        QString line = input.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(separator); // последний столбец отделен сепаратором, его отделяем в отдельный массив
        if (parts.size() != 2)
        {
            return false;
        }

        QStringList numbers = parts[0].trimmed().split(QRegExp("\\s+"), QString::SkipEmptyParts);
        QVector<double> row;
        for (const QString& numStr : numbers)
        {
            bool ok;
            double num = numStr.toDouble(&ok);
            if (ok) row.append(num);
        }

        if (!row.isEmpty())
        {
            this->X.append(row);

            bool ok;
            double Last_Val = parts[1].trimmed().toDouble(&ok);
            if (ok) this->Y.append(Last_Val);
        }
    }
    file.close();
    return true;
}
