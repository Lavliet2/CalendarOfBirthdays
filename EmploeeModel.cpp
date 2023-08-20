#include "EmploeeModel.h"
#include <QDebug>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QTextCharFormat>

#include <QDate>
#include <QStringList>



EmploeeModel::EmploeeModel(QObject *parent)
    : QAbstractTableModel(parent)
    , filePath("C:/temp/database.txt")
//    , filePath("O:/06. Данные отделов/НИО ММНМ/Рабочие данные/Собственные программы/Murzin_AV/Base/birthdays.txt")

{
    // Инициализация заголовков столбцов
    headers << "Фамилия" << "Имя" << "Отчество" << "Должность" << "Дата рождения" << "Почта";
    fillDataList();
}


int EmploeeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return emploeeList.count();
}


int EmploeeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return headers.size();
}


QVariant EmploeeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {

        switch (index.column())
        {
        case EColumn::Famili:
            return emploeeList[index.row()]->famili;
        case EColumn::Name:
            return emploeeList[index.row()]->name;
        case EColumn::Otch:
            return emploeeList[index.row()]->otch;
        case EColumn::Post:
            return emploeeList[index.row()]->post;
        case EColumn::Birthday:
        {
            // Преобразование строки в QDate
            QDate birthDate = QDate::fromString(emploeeList[index.row()]->birthday, "dd.MM.yyyy");
            if ( birthDate.isValid() ) return birthDate.toString("dd.MM.yyyy"); // Форматирование QDate обратно в строку для отображения
            else                       return QVariant();                       // Некорректная дата
        }
//            return emploeeList[index.row()]->birthday;
        case EColumn::Email:
            return emploeeList[index.row()]->email;
        }
    }
    if ( role == Qt::ToolTipRole && index.column() == Birthday ) {
        QDate birthDate = QDate::fromString(emploeeList[index.row()]->birthday, "dd.MM.yyyy");
        if ( birthDate.isValid() ) {
            int age = calculateAge(birthDate);
            return "Возраст: " + QString::number(age);
        }
        else return "Invalid birth date";
    }

    return QVariant();
}


QVariant EmploeeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section >= 0 && section < headers.size())
    {
        return headers[section];
    }

    return QVariant();
//    return QAbstractTableModel::headerData(section, orientation, role);
}


bool EmploeeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        switch (index.column())
        {
        case EColumn::Famili:
            emploeeList[index.row()]->famili  = value.toString();
            break;
        case EColumn::Name:
            emploeeList[index.row()]->name = value.toString();
            break;
        case EColumn::Otch:
            emploeeList[index.row()]->otch  = value.toString();
            break;
        case EColumn::Post:
            emploeeList[index.row()]->post  = value.toString();
            break;

        //доработать, странная ошибка
        case EColumn::Birthday:
            {
//            emploeeList[index.row()]->birthday  = value.toString();
//            /*
                QString birthday = value.toString();

                QDate date = QDate::fromString(birthday, "dd.MM.yyyy");
                if (date.isValid()) {
                    emploeeList[index.row()]->birthday = birthday;
                } else {
                    QMessageBox::warning(nullptr, "Предупреждение", "Поле <<ДЕНЬ РОЖДЕНИЯ>> заполнено некорректно");
                    return false;
                }
//                */
            }
            break;

        case EColumn::Email:
            {
                QString email = value.toString();
                if ( email.contains("@") && email.contains(".") ) {
                    emploeeList[index.row()]->email = email;
                } else {
                    QMessageBox::warning(nullptr, "Предупреждение", "Поле <<Почта>> заполнено некорректно, отсутсвуют специальные символы <@><.>");
                    return false;
                }
            }
            break;
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}


Qt::ItemFlags EmploeeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}


bool EmploeeModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    emploeeList.removeAt(row);

    return true;
}

void EmploeeModel::addData(Employee *emploee)
{
//     Employee *empl = new Employee;
    beginInsertRows(QModelIndex(), emploeeList.size(), emploeeList.size());
    emploeeList.append(emploee);
    endInsertRows();
}

void EmploeeModel::sort(int column, Qt::SortOrder order, bool sortBy)
{
//    if ( column == Birthday || column == Famili ) { // Замените на номер столбца с датами рождения
        sortOrder = order;
        sortColumn = column;
        sortData(sortBy);
//    } else QAbstractTableModel::sort(column, order);

}

void EmploeeModel::fillDataList()
{
    // Открываем файл
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл";
        return;
    }
    // Читаем информацию из файла
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QStringList splitLine = in.readLine().split('\t');

        Employee *empl = new Employee;

        empl->famili   = splitLine[Famili];
        empl->name     = splitLine[Name];
        empl->otch     = splitLine[Otch];
        empl->post     = splitLine[Post];
        empl->birthday = splitLine[Birthday];
        empl->email    = splitLine[Email];

        emploeeList.append(empl);
    }
    // Закрываем файл
    file.close();
}

bool EmploeeModel::compareDates(const Employee *a, const Employee *b)
{
    QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
    QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

    if (!dateA.isValid() || !dateB.isValid())
        return false;

    QDate currentDate = QDate::currentDate();

    QDate modifiedDateA(currentDate.year(), dateA.month(), dateA.day());
    QDate modifiedDateB(currentDate.year(), dateB.month(), dateB.day());

    if (sortOrder == Qt::AscendingOrder)
        return modifiedDateA < modifiedDateB;
    else
        return modifiedDateA > modifiedDateB;
}

bool EmploeeModel::compareMonthAndDay(const QDate &dateA, const QDate &dateB)
{
    if (sortOrder == Qt::AscendingOrder)
        return (dateA.month() < dateB.month()) || (dateA.month() == dateB.month() && dateA.day() < dateB.day());
    else
        return (dateA.month() > dateB.month()) || (dateA.month() == dateB.month() && dateA.day() > dateB.day());
}

//bool EmploeeModel::compareNames(const Employee *a, const Employee *b)
//{

//    if (sortOrder == Qt::AscendingOrder)
//        return a->famili < b->famili;
//    else
//        return a->famili > b->famili;
//}

int EmploeeModel::daysUntilBirthday(const QDate &date)
{
    QDate currentDate = QDate::currentDate();
    QDate nextBirthday(currentDate.year(), date.month(), date.day());

    if (nextBirthday < currentDate)
        nextBirthday = nextBirthday.addYears(1);

    return currentDate.daysTo(nextBirthday);
}

void EmploeeModel::sortData(bool sortBy)
{
    if ( sortBy ) {
        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
            QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
            QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

            if (!dateA.isValid() || !dateB.isValid())
                return false;

            int daysUntilBirthdayA = daysUntilBirthday(dateA);
            int daysUntilBirthdayB = daysUntilBirthday(dateB);

            if (sortOrder == Qt::AscendingOrder)
                return daysUntilBirthdayA < daysUntilBirthdayB;
            else
                return daysUntilBirthdayA > daysUntilBirthdayB;
        });
    }
    else {
    switch ( sortColumn )
    {
    case Famili: case Name: case Otch: case Post: case Email:
        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
//            return compareNames(a, b);
            if (sortOrder == Qt::AscendingOrder) {
                if      ( sortColumn == Famili)
                    return a->famili < b->famili;
                else if ( sortColumn == Name)
                    return a->name < b->name;
                else if ( sortColumn == Otch)
                    return a->otch < b->otch;
                else if ( sortColumn == Post)
                    return a->post < b->post;
                else if ( sortColumn == Email)
                    return a->email < b->email;

            }
            else
                if      ( sortColumn == Famili)
                    return a->famili > b->famili;
                else if ( sortColumn == Name)
                    return a->name > b->name;
                else if ( sortColumn == Otch)
                    return a->otch > b->otch;
                else if ( sortColumn == Post)
                    return a->post > b->post;
                else if ( sortColumn == Email)
                    return a->email > b->email;

            return  a->famili > b->famili; //хз
        });

        break;
    case Birthday:
    {
        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
            QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
            QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

            if (!dateA.isValid() || !dateB.isValid())
                return false;

            if (sortOrder == Qt::AscendingOrder)
                return dateA < dateB;
            else
                return dateA > dateB;
        });

//        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
//            QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
//            QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

//            if (!dateA.isValid() || !dateB.isValid())
//                return false;

//            int daysUntilBirthdayA = daysUntilBirthday(dateA);
//            int daysUntilBirthdayB = daysUntilBirthday(dateB);

//            if (sortOrder == Qt::AscendingOrder)
//                return daysUntilBirthdayA < daysUntilBirthdayB;
//            else
//                return daysUntilBirthdayA > daysUntilBirthdayB;
//        });
    }
    default:
        break;
    }
    }
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    /*
    if (sortColumn == Famili )
    {
        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
            return compareNames(a, b);
        });
    }
    else if (sortColumn == Birthday) {

        std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
            QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
            QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

            if (!dateA.isValid() || !dateB.isValid())
                return false;

            int daysUntilBirthdayA = daysUntilBirthday(dateA);
            int daysUntilBirthdayB = daysUntilBirthday(dateB);

            if (sortOrder == Qt::AscendingOrder)
                return daysUntilBirthdayA < daysUntilBirthdayB;
            else
                return daysUntilBirthdayA > daysUntilBirthdayB;
        });
    }
        emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
    */




//    QDate currentDate = QDate::currentDate();

//    std::sort(emploeeList.begin(), emploeeList.end(), [this, &currentDate](const Employee *a, const Employee *b) {
//        QDate dateA = QDate::fromString(a->birthday, "dd.MM.yyyy");
//        QDate dateB = QDate::fromString(b->birthday, "dd.MM.yyyy");

//        if (!dateA.isValid() || !dateB.isValid())
//            return false;

//        QDate modifiedDateA(currentDate.year(), dateA.month(), dateA.day());
//        QDate modifiedDateB(currentDate.year(), dateB.month(), dateB.day());

//        return compareMonthAndDay(modifiedDateA, modifiedDateB);
//    });

//    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));


//    std::sort(emploeeList.begin(), emploeeList.end(), [this](const Employee *a, const Employee *b) {
//        return compareDates(a, b);
//    });
    //    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

int EmploeeModel::calculateAge(const QDate &birthDate) const
{
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - birthDate.year();

    if (birthDate.month() > currentDate.month() ||
        (birthDate.month() == currentDate.month() && birthDate.day() > currentDate.day()))
    {
        age--;
    }

    return age;
}


