#include "mainwindow.h"
#include "ui_mainwindow.h"

double num_first;
QString expression;



MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ЧИСЛОВЫЕ ОПЕРАЦИИ
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));



    // МАТЕМАТИЧЕСКИЕ ОПЕРАЦИИ //
    connect(ui->pushButton_divide, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->pushButton_multi, SIGNAL(clicked()), this, SLOT(math_operations()));
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(math_operations()));


    //connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(operations()));

    ui->pushButton_divide->setCheckable(true);
    ui->pushButton_minus->setCheckable(true);
    ui->pushButton_multi->setCheckable(true);
    ui->pushButton_plus->setCheckable(true);
    //-------------------------//

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Ввод с клавиатуры
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_0){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "0").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_1){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "1").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_2){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "2").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_3){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "3").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_4){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "4").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_5){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "5").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_6){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "6").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_7){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "7").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_8){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "8").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_9){
        double all_numbers;
        QString new_label;
        all_numbers = (ui->result_show->text() + "9").toDouble();
        new_label = QString::number(all_numbers, 'g', 15);
        ui->result_show->setText(new_label);
    }
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        double labelNumber, num_second;
        QString new_label;

        num_second = ui->result_show->text().toDouble();

        if(ui->pushButton_plus->isChecked()) {
            labelNumber = num_first + num_second;
            new_label = QString::number(labelNumber, 'g', 15);
            ui->result_show->setText(new_label);
            ui->pushButton_plus->setChecked(false);
        } else if(ui->pushButton_minus->isChecked()) {
            labelNumber = num_first - num_second;
            new_label = QString::number(labelNumber, 'g', 15);
            ui->result_show->setText(new_label);
            ui->pushButton_minus->setChecked(false);
        } else if(ui->pushButton_multi->isChecked()) {
            labelNumber = num_first * num_second;
            new_label = QString::number(labelNumber, 'g', 15);
            ui->result_show->setText(new_label);
            ui->pushButton_multi->setChecked(false);
        } else if(ui->pushButton_divide->isChecked()) {
            if (num_second == 0) {
                ui->result_show->setText("0");
            } else {
            labelNumber = num_first / num_second;
            new_label = QString::number(labelNumber, 'g', 15);
            ui->result_show->setText(new_label);
            }
            ui->pushButton_divide->setChecked(false);
        }
    }
    if (e->key() == Qt::Key_Minus) {
        num_first = ui->result_show->text().toDouble();
        ui->result_show->setText("");
        ui->pushButton_minus->setChecked(true);
    }
    if (e->key() == Qt::Key_Plus) {
        num_first = ui->result_show->text().toDouble();
        ui->result_show->setText("");
        ui->pushButton_plus->setChecked(true);
    }
    if (e->key() == Qt::Key_Asterisk) {
        num_first = ui->result_show->text().toDouble();
        ui->result_show->setText("");
        ui->pushButton_multi->setChecked(true);
    }
    if (e->key() == Qt::Key_Slash) {
        num_first = ui->result_show->text().toDouble();
        ui->result_show->setText("");
        ui->pushButton_divide->setChecked(true);
    }
    if (e->key() == Qt::Key_Period) {
        if(!(ui->result_show->text().contains('.')))
            ui->result_show->setText(ui->result_show->text() + '.');
    }
}


void MainWindow::digits_numbers(){
    QPushButton *button = static_cast<QPushButton *>(sender());

    double all_numbers;
    QString new_label;

    //all_numbers = (ui->label->text() + button->text()).toDouble();
    //new_label = QString::number(all_numbers);
    //ui->label->setText(new_label);

    if (ui->result_show->text().contains(".") && button->text() == 0) {
        new_label = ui->result_show->text() + button->text();
    } else {
        all_numbers = (ui->result_show->text() + button->text()).toDouble();
        new_label = QString::number(all_numbers, 'g', 15);

    }

    ui->result_show->setText(new_label);
}

// Символ точка
void MainWindow::on_pushButton_dot_clicked()
{
    if(!(ui->result_show->text().contains('.'))) {
        ui->result_show->setText(ui->result_show->text() + '.');
    }
}

// Математические операции
void MainWindow::math_operations()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    num_first = ui->result_show->text().toDouble();
    ui->result_show->setText("");
    button->setChecked(true);
}

// Операция "равно"
void MainWindow::on_pushButton_equal_clicked()
{
    double labelNumber, num_second;
    QString new_label;

    num_second = ui->result_show->text().toDouble();


    if(ui->pushButton_plus->isChecked()) {
        labelNumber = num_first + num_second;
        new_label = QString::number(labelNumber, 'g', 15);
        ui->result_show->setText(new_label);
        ui->pushButton_plus->setChecked(false);
    } else if(ui->pushButton_minus->isChecked()) {
        labelNumber = num_first - num_second;
        new_label = QString::number(labelNumber, 'g', 15);
        ui->result_show->setText(new_label);
        ui->pushButton_minus->setChecked(false);
    } else if(ui->pushButton_multi->isChecked()) {
        labelNumber = num_first * num_second;
        new_label = QString::number(labelNumber, 'g', 15);
        ui->result_show->setText(new_label);
        ui->pushButton_multi->setChecked(false);
    } else if(ui->pushButton_divide->isChecked()) {
        if (num_second == 0) {
            ui->result_show->setText("0");
        } else {
        labelNumber = num_first / num_second;
        new_label = QString::number(labelNumber, 'g', 15);
        ui->result_show->setText(new_label);
        }
        ui->pushButton_divide->setChecked(false);
    }
}


void MainWindow::on_pushButton_clear_clicked()
{
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_divide->setChecked(false);
    ui->pushButton_multi->setChecked(false);

    ui->result_show->setText("0");
}
