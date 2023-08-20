#ifndef EMPLOEEMODEL_H
#define EMPLOEEMODEL_H

#include <QAbstractTableModel>

struct Employee
{
    QString famili;
    QString name;
    QString otch;
    QString post;
    QString birthday;
    QString email;
};

class EmploeeModel : public QAbstractTableModel
{

public:
    enum EColumn {
        Famili,
        Name,
        Otch,
        Post,
        Birthday,
        Email
    };

    EmploeeModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool     setData(const QModelIndex &index, const QVariant &value, int role) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

   // bool removeRows(int row, int count, const QModelIndex &parent)override;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    void addData(Employee *emploee);
    void sort(int column, Qt::SortOrder order, bool sortBy = false);//override;

private:
    void fillDataList();
    void sortData(bool sortBy = false);

    int calculateAge(const QDate &birthDate) const;

    bool compareDates(const Employee *a, const Employee *b);
    bool compareMonthAndDay(const QDate &dateA, const QDate &dateB);
//    bool compareNames(const Employee *a, const Employee *b);

    static int daysUntilBirthday(const QDate &date);

//    void saveData();

public:
    Qt::SortOrder sortOrder = Qt::AscendingOrder;

private:
    QString filePath;
    QStringList dataList;

    QList<Employee *> emploeeList;
    QStringList headers; // Заголовки столбцов

    int sortColumn = -1;


};

#endif // EMPLOEEMODEL_H
