#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AddUserForm.h"
#include "EmploeeModel.h"

#include <QDate>
#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
//    enum EmpoeeColumn {
//        Name,
//        Famili,
//        Otch,
//        Post,
//        Birthday,
//        Email
//    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    void readText();
//    void writeText(const QStringList &lines);
    void dateOgonki();

    bool confirmDeletion();
    void removeRowFromModel(int row);
    void removeRowFromFile(int row);

private:
    void init();

private slots:
    void removeLineFromFile(const QString& filename, int lineIndex);

    void handleHeaderClicked(int logicalIndex);

    void on_pb_Add_clicked();
    void on_pb_Del_clicked();
    void on_pb_Save_clicked();

    void on_pb_Sort_clicked();

private:
    Ui::MainWindow *ui;
//    QStringList dataList;
//    Employee *emploee;
    QString filePath;
    EmploeeModel *eModel = nullptr;
    int sortClickCount = 0;
};
#endif // MAINWINDOW_H
