#include "AddUserForm.h"
#include "ui_AddUserForm.h"
#include <QDebug>
#include <QMessageBox>

#include "EmploeeModel.h"

//struct Employee;

AddUserForm::AddUserForm(Employee &employer, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddUserForm)
    , emploee(employer)
{
    ui->setupUi(this);
}

void AddUserForm::done(int result)
{
//    QDate d = QDate::fromString(ui->dateEdit->date().toString(), "dd.MM.yyyy");

    QStringList errorList;
//    QString name = ui->editline_Name->text();


    emploee.name = ui->editline_Name->text();
    emploee.famili = ui->editline_Famili->text();
    emploee.otch = ui->lineEdit_Otch->text();
    emploee.post = ui->lineEdit_Post->text();
    emploee.birthday = ui->dateEdit->date().toString("dd.MM.yyyy");
//    emploee.birthday = QString("%1.%2.%3")
////            .arg(ui->dateEdit->date().day()  <= 10 ?  0 + ui->dateEdit->date().day()   : ui->dateEdit->date().day())
////            .arg(ui->dateEdit->date().month()<= 10 ?  0 + ui->dateEdit->date().month() : ui->dateEdit->date().month() )
//            .arg(ui->dateEdit->date().day())
//            .arg(ui->dateEdit->date().month())
//            .arg(ui->dateEdit->date().year());
    emploee.email = ui->lineEdit_Email->text();

//    QString email = ui->lineEdit_Email->text();
//    if (email.contains("@")) {
//        emploee.email = email;
//    } else {
//        QMessageBox::warning(nullptr, "Предупреждение", "Поле <<ПОЧТА>> заполнено некорректно");
//        return;
//    }



    if ( emploee.name.isEmpty()     )  errorList << "Поле <<Имя>> пустое";
    if ( emploee.famili.isEmpty()   )  errorList << "Поле <<Фамилия>> пустое";
    if ( emploee.otch.isEmpty()     )  errorList << "Поле <<Отчество>> пустое";
    if ( emploee.post.isEmpty()     )  errorList << "Поле <<Должность>> пустое";
    if ( emploee.birthday.isEmpty() )  errorList << "Поле <<Дата рождения>> пустое";
    if ( emploee.email.isEmpty()    )  errorList << "Поле <<Поле>> пустое";
    if (!emploee.email.contains("@") || !emploee.email.contains(".")  )
        errorList    << "Поле <<Почта>> заполнено некорректно, отсутствуют специальные символы <@><.>";

    if ( !errorList.isEmpty() ) {

        QMessageBox::warning(nullptr, "Предупреждение", errorList.join(";\n"));
        return;
    }


    QDialog::done(result);
    setOkEnabled();

}

AddUserForm::~AddUserForm()
{
    delete ui;
}

void AddUserForm::setOkEnabled()
{
    ui->buttonBox->setEnabled(emploee.name.isEmpty()   ||
                              emploee.famili.isEmpty() ||
                              emploee.otch.isEmpty()   ||
                              emploee.post.isEmpty()   ||
                              emploee.email.isEmpty()  ||
                              emploee.birthday.isEmpty());
}
