#ifndef ADDUSERFORM_H
#define ADDUSERFORM_H

#include <QDialog>
#include "QDate"

struct Employee;

namespace Ui {
class AddUserForm;
}

class AddUserForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserForm(Employee &emploee, QWidget *parent = nullptr);
    virtual void done(int result)override;
    ~AddUserForm();

private slots:
    void setOkEnabled();

private:

    Ui::AddUserForm *ui;
    Employee &emploee;

};

#endif // ADDUSERFORM_H
