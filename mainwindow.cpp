#include "mainwindow.h"
#include "AddUserForm.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDate>
#include <QMessageBox>
#include <QTextCharFormat>



#include <QStylePainter>
#include <QApplication>
#include <QCalendarWidget>
#include <QHeaderView>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
//    , emploee(new Employee)
    , filePath("C:/temp/database.txt")
//    , filePath("O:/06. Данные отделов/НИО ММНМ/Рабочие данные/Собственные программы/Murzin_AV/Base/birthdays.txt")
{
    ui->setupUi(this);
    init();


    eModel = new EmploeeModel(this);
    ui->tableView->setModel(eModel);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::handleHeaderClicked);


    dateOgonki();
    connect(ui->tableView->model(), &QAbstractTableModel::dataChanged, [&]() {
        dateOgonki();
        if ( senderSignalIndex() == EmploeeModel::EColumn::Birthday )  {

        }
        /*
        auto indexes = ui->tableView->selectionModel()->selectedColumns();

        for ( auto a : indexes ) {
            if ( a.column() == EmploeeModel::modelColumn::Birthday ) {
                dateOgonki();
            }
        }
*/

    });
}

MainWindow::~MainWindow()
{
//    delete emploee;
    delete ui;
}

void MainWindow::dateOgonki()
{
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QDate currentDate = QDate::currentDate();

    for ( int i = 0; i < eModel->rowCount(); i++ ) {
        QModelIndex index = eModel->index(i, EmploeeModel::EColumn::Birthday);
        QString birthday = eModel->data(index, Qt::EditRole).toString();

        QStringList date = birthday.split('.');
        QDate eventDate(currentDate.year(), date[1].toInt(), date[0].toInt());

        index = eModel->index(i, EmploeeModel::EColumn::Name);
        QString name = eModel->data(index, Qt::EditRole).toString();
        index = eModel->index(i, EmploeeModel::EColumn::Famili);
        QString famili = eModel->data(index, Qt::EditRole).toString();
        index = eModel->index(i, EmploeeModel::EColumn::Otch);
        QString otch = eModel->data(index, Qt::EditRole).toString();

        QTextCharFormat textCharFormat = ui->calendarWidget->dateTextFormat(eventDate);
        textCharFormat.setToolTip(/*textCharFormat.toolTip() +*/famili+" "+ name+" "+ otch + "\n");
        textCharFormat.setForeground(Qt::green);

        ui->calendarWidget->setDateTextFormat(eventDate, textCharFormat);

        QDate nextEventDate = eventDate.addYears(1);
        while ( nextEventDate <= currentDate ) {
            nextEventDate = nextEventDate.addYears(1);
        }

        if ( nextEventDate.year() != eventDate.year() )  {
            textCharFormat = ui->calendarWidget->dateTextFormat(nextEventDate);
            textCharFormat.setToolTip(textCharFormat.toolTip() + name + "\n");
            textCharFormat.setForeground(Qt::green);

            ui->calendarWidget->setDateTextFormat(nextEventDate, textCharFormat);
        }
    }
}

void MainWindow::removeLineFromFile(const QString &filename, int lineIndex)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        QString line;
        QStringList lines;

        // Читаем все строки из файла
        while (!stream.atEnd())
        {
            line = stream.readLine();
            lines.append(line);
        }

        // Удаляем строку с указанным индексом
        if (lineIndex >= 0 && lineIndex < lines.size())
        {
            lines.removeAt(lineIndex);
        }

        // Перезаписываем файл с обновленными строками
        file.resize(0);
        for (const QString &updatedLine : lines)
        {
            stream << updatedLine << Qt::endl;
        }
    }
    file.close();
}

void MainWindow::handleHeaderClicked(int logicalIndex)
{
//    if (logicalIndex == EmploeeModel::Birthday  || logicalIndex == EmploeeModel::Famili)
//    {
        Qt::SortOrder newSortOrder = (eModel->sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
        eModel->sort(logicalIndex, newSortOrder);
//    }
}

bool MainWindow::confirmDeletion()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение", "Вы уверены, что хотите удалить эту запись?",
                                  QMessageBox::Yes | QMessageBox::No);
    return reply == QMessageBox::Yes;
}

void MainWindow::removeRowFromModel(int row)
{
    eModel->removeRow(row);
    eModel->layoutChanged();
    ui->tableView->setCurrentIndex(QModelIndex());
    ui->tableView->update();
}

void MainWindow::removeRowFromFile(int row)
{
    QString fileName = filePath;
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream in(&file);
        QStringList lines;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            lines.append(line);
        }
        file.resize(0); // Очищаем файл

        for (const QString &line : lines)
        {
            QStringList words = line.split('\t');
            if (words.isEmpty())
                continue;
            int currentRow = words[0].toInt();
            if (currentRow != row)
            {
                in << line << Qt::endl;
            }
        }
        file.close();
    }
}

void MainWindow::init()
{
    QApplication::setStyle("Fusion");

    // Настройка темы для тёмного стиля
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    QApplication::setPalette(darkPalette);

    QFile styleFile(":/src/resources/Styles/calendar_style.qss");
    if ( styleFile.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        QString style = styleFile.readAll();
        styleFile.close();
        ui->calendarWidget->setStyleSheet(style);
        ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    }

       QString style = "QTableView:!active {"
                "  selection-background-color: #191919;"
                "  selection-color: white;"
                "}"
                "QTableView:active {"
                "  selection-background-color: #353535;"
                "  selection-color: white;"
                "}";

        ui->tableView->setStyleSheet(style);
    // Настройка стиля для QCalendarWidget
//    QString style = "QCalendarWidget {"
//                   "  background-color: #333;"
//                   "  color: white;"
//                   "}"
//                   "QCalendarWidget QAbstractItemView {"
//                   "  background-color: #444;"
//                   "  color: white;"
//                   "}"
//                   "QCalendarWidget QToolButton {"
//                   "  background-color: #555;"
//                   "  color: white;"
//                   "}"
//                   "QCalendarWidget QToolButton:hover {"
//                   "  background-color: #666;"
//                   "}"
//                    "QCalendarWidget QTableView:!active {"
//                                       "  selection-background-color: #444;"
//                                       "  selection-color: black;"
//                                       "}"
//                    "QCalendarWidget QTableView:active {"
//                                       "  selection-background-color: #95a5a6;"
//                                       "  selection-color: black;"
//                                       "}";
//    ui->calendarWidget->setNavigationBarVisible(false);




}

void MainWindow::on_pb_Add_clicked()
{
    // Создание и открытие новой формы для добавления данных
    Employee *emploee = new Employee;
    AddUserForm *addUserForm = new AddUserForm(*emploee);
    addUserForm->setWindowFlags(addUserForm->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    if ( addUserForm->exec() == QDialog::Accepted ) {
        // Добавление данных в модель
        eModel->addData(emploee);
        dateOgonki();


        /*
        // Сохранение данных в файл
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            qDebug() << "Не удалось открыть файл для записи";
            return;
        }
        QTextStream out(&file);
        out << emploee->name     << "\t"
            << emploee->famili   << "\t"
            << emploee->otch     << "\t"
            << emploee->post     << "\t"
            << emploee->birthday << "\t"
            << emploee->email    << Qt::endl;
        file.close();
        */
    }
}

void MainWindow::on_pb_Del_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if (row >= 0)
    {
        if (confirmDeletion())
        {
            removeRowFromModel(row);
            removeRowFromFile(row);
        }
    }
}

void MainWindow::on_pb_Save_clicked()
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Не удалось открыть файл для записи";
        return;
    }

    QTextStream out(&file);

    for (int row = 0; row < eModel->rowCount(); row++)
    {
        for (int col = 0; col < eModel->columnCount(); col++)
        {
            QModelIndex index = eModel->index(row, col);
            out << eModel->data(index).toString() + '\t';
        }
        out << Qt::endl;
    }

    file.close();
}

void MainWindow::on_pb_Sort_clicked()
{
    if ( sortClickCount % 2 == 0)
        eModel->sort(EmploeeModel::Birthday, Qt::AscendingOrder, true);
    else
        eModel->sort(EmploeeModel::Birthday, Qt::DescendingOrder, true);
    sortClickCount++;
}

