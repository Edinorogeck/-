#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

QString expression = "";
qint16 numBrackets = 0;


// Тригеры возможных операций:
//     true - разрешено вставить
//     false - запрещено вставить
bool numberTrigger = true;
bool leftBracketTrigger = true;
bool rightBracketTrigger = false;
bool mathOperationTrigger = false;
bool dotTrigger = false;
bool negativeSymbolTrigger = true;
bool exponentTrigger = false;

// Спец триггер на "0"
bool nullTrigger = true;
//Спец триггер на дробность числа:
//    true - число дробное
//    false - число не дробное
bool isNumberIsDouble = false;

// Принятое json сообщение
QString jsonInString = "";

// Массив json объектов для истории вычислений
QJsonArray historyJsonArray;



//--- Для таблицы истории ---//
// Объявление модели данных(таблица)
QStandardItemModel* logTableModel;
// Объявление списка значений столбцов таблицытаблицы
QList<QStandardItem*> row;
// Объявление списка строк таблицы
QList<QList<QStandardItem*>> rowList;
//--------------------------//


void MainWindow::connectToHost()
{
    socket = new QTcpSocket;
    socket->connectToHost("127.0.0.1", 2323);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    nextBlockSize = 0;
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_6);
    if (in.status() == QDataStream::Ok) {
        jsonInString = socket->readAll();
        MessageHandler(jsonInString);

//        QJsonDocument jsonInDoc = QJsonDocument::fromJson(jsonInString.toUtf8());
//        QJsonObject jsonInObj = jsonInDoc.object();
//        expression = jsonInObj["result"].toString();

//        ui->result_show->setText(expression);




//        for (;;) {
//            if (nextBlockSize == 0) {
//                if (socket->bytesAvailable() < 2) {
//                    break;
//                }
//                in >> nextBlockSize;
//            }
//            if (socket->bytesAvailable() < nextBlockSize) {
//                break;
//            }
//            QString str;
//            in >> str;
//            nextBlockSize = 0;

//            // Тестовый вывод //
//            ui->result_show->setText(str);
//            //ui->result_show->setText("СЛОТ РЕДИ РИД");

//            ui->result_show->setText(socket->readAll());

//        }

    } else {
        ui->result_show->setText("read error");
    }
}


// Функция отправки на сервер //
//void MainWindow::SendToServer(QString str)
//{
//    QDateTime currentTime = QDateTime::currentDateTime();

//    QJsonObject jsonObject;
//    jsonObject["time_of_click"] = currentTime.toString(Qt::ISODate);
//    jsonObject["expression"] = expression;
//    jsonObject["klick"] = str;

//    QJsonDocument jsonDocument(jsonObject);
//    QString jsonString = jsonDocument.toJson();


//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_6);
//    out << quint16(0) << jsonString;
//    out.device()->seek(0);
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
//}

void MainWindow::SendToServer(QPushButton *klick, bool calculate = false)
{
    QJsonObject jsonObject;
    QDateTime currentTime = QDateTime::currentDateTime();

    QString clickString = "button: " + klick->text();

    jsonObject["expression"] = expression;
    jsonObject["klick"] = clickString;
    jsonObject["time_of_click"] = currentTime.toString(Qt::ISODate);
    jsonObject["calculate"] = calculate;

    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson();

//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_6);
//    out << quint16(0) << jsonString;
//    out.device()->seek(0);
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);

    socket->write(jsonString.toUtf8());
}

void MainWindow::SendToServer(QKeyEvent *klick, bool calculate = false)
{
    QJsonObject jsonObject;
    QDateTime currentTime = QDateTime::currentDateTime();

    QString clickString = "key: " + klick->text();

    jsonObject["expression"] = expression;
    jsonObject["klick"] = clickString;
    jsonObject["time_of_click"] = currentTime.toString(Qt::ISODate);
    jsonObject["calculate"] = calculate;

    QJsonDocument jsonDocument(jsonObject);
    QString jsonString = jsonDocument.toJson();

//    Data.clear();
//    QDataStream out(&Data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_6);
//    out << quint16(0) << jsonString;
//    out.device()->seek(0);
//    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
    socket->write(jsonString.toUtf8());
}

void MainWindow::MessageHandler(QString jsonInString)
{
    QJsonDocument jsonInDoc = QJsonDocument::fromJson(jsonInString.toUtf8());
    QJsonObject jsonInObj = jsonInDoc.object();

    historyJsonArray.append(jsonInObj);

    //expression = historyJsonArray.last()["result"].toString();

    expression = jsonInObj["result"].toString();

    ui->result_show->setText(expression);
    RefreshHistoryTable();

}

void MainWindow::RefreshHistoryTable()
{
    // Можно переделать не под циклы, а под вывод последнего элементамассива,
    // под добавление строки в конец и ее вывод на экран через "ui->tableView->update()".

    rowList.clear();

    for (int i = 0; i < historyJsonArray.size(); ++i) {
        row.clear();
        QStandardItem* item0 = new QStandardItem(historyJsonArray.at(i)["number"].toString());
        item0->setEditable(false);
        QStandardItem* item1 = new QStandardItem(historyJsonArray.at(i)["time_of_click"].toString());
        item1->setEditable(false);
        QStandardItem* item2 = new QStandardItem(historyJsonArray.at(i)["get_time"].toString());
        item2->setEditable(false);
        QStandardItem* item3 = new QStandardItem(historyJsonArray.at(i)["expression"].toString());
        item3->setEditable(false);
        QStandardItem* item4 = new QStandardItem(historyJsonArray.at(i)["result"].toString());
        item4->setEditable(false);
        QStandardItem* item5 = new QStandardItem(historyJsonArray.at(i)["calculation_duration"].toString());
        item5->setEditable(false);

        row << item0 << item1 << item2 << item3 << item4 << item5;
        rowList << row;
    }

    logTableModel->removeRows(0, logTableModel->rowCount());

    for (int i = 0; i < rowList.size(); ++i) {
        logTableModel->appendRow(rowList.at(i));
    }

    ui->tableView->setModel(logTableModel);
}

void MainWindow::MakeHistoryTable()
{
    // ---------- History header ---------- //
    QStringList headers;
    headers << QChar(0x2116) << "Время\nформирования\nсообщения"   //
            << "Время\nполучения\nсервером" << "Выражение"         // Список заголовков таблицы
            << "Результат" << "Длительность\nвычисления";          //

    logTableModel = new QStandardItemModel();



    logTableModel->setHorizontalHeaderLabels(headers); // Установка заголовков столбцов модели данных
    ui->tableView->setModel(logTableModel); // Установка модели данных в TableView
    ui->tableView->setFocusPolicy(Qt::NoFocus); // Убирает фокус с клавиатуры для TableView

    // Настройка внешнего вида
    ui->tableView->horizontalHeader()->setSectionsClickable(false); // Запрет хедеру на реагирование на щелчки мыши
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Установка на выделение всей строки по щелчку мыши
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(5);


    // Удаление автонумерации таблицы истории
    ui->tableView->verticalHeader()->hide();


    //-------------------------------------//
}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    socket = new QTcpSocket;
//    socket->connectToHost("127.0.0.1", 2323);
//    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
//    nextBlockSize = 0;
    connectToHost();

    MakeHistoryTable();


    // ЧИСЛОВЫЕ ОПЕРАЦИИ
    //connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
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


//    connect(ui->pushButton_insert_history_result, SIGNAL(clicked()), this, SLOT(on_pushButton_insert_history_result_clicked()));

//    ui->pushButton_divide->setCheckable(true);
//    ui->pushButton_minus->setCheckable(true);
//    ui->pushButton_multi->setCheckable(true);
//    ui->pushButton_plus->setCheckable(true);
    //-------------------------//

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::digits_numbers(){
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (numberTrigger && isNumberIsDouble) {
        expression = expression + button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = true;
        mathOperationTrigger = true;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = true;
        nullTrigger = true;
    } else if (numberTrigger) {
        expression = expression + button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = true;
        mathOperationTrigger = true;
        dotTrigger = true;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = true;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::on_pushButton_0_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

   //if (expression.size() > 0 && !isNumberIsDouble && (expression.right(1).at(0).isDigit() && expression.right(1) != "0")) {
    if (expression.size() > 0 && !isNumberIsDouble && (expression.right(1).at(0).isDigit())) {
        expression += button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = true;
        mathOperationTrigger = true;
        dotTrigger = true;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = false;
    } else if (isNumberIsDouble && nullTrigger) {
        expression += button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = true;
        mathOperationTrigger = true;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = true;
        nullTrigger = true;
    } else if (nullTrigger) {
        expression += button->text();

        numberTrigger = false;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = true;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = false;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::math_operations()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (mathOperationTrigger) {
        expression = expression + " " + button->text() + " ";

        numberTrigger = true;
        leftBracketTrigger = true;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = true;
        exponentTrigger = false;
        nullTrigger = true;

        isNumberIsDouble = false;
    }
    if (expression.size() > 0 && mathOperationTrigger == false) {
        if (expression.right(3) == " - " || expression.right(3) == " + "
                || expression.right(3) == " * " || expression.right(3) == " / ") {
            expression.chop(3);

            expression = expression + " " + button->text() + " ";

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::on_pushButton_left_bracket_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (leftBracketTrigger) {
        expression = expression + button->text();
        numBrackets += 1;

        numberTrigger = true;
        leftBracketTrigger = true;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = true;
        exponentTrigger = false;
        nullTrigger = true;

        isNumberIsDouble = false;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::on_pushButton_right_bracket_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (numBrackets > 0 && rightBracketTrigger) {
        expression = expression + button->text();

        numBrackets -= 1;

        numberTrigger = false;
        leftBracketTrigger = false;
        rightBracketTrigger = true;
        mathOperationTrigger = true;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = false;

        isNumberIsDouble = false;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::on_pushButton_dot_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (!isNumberIsDouble && dotTrigger) {
        expression = expression + button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = true;

        isNumberIsDouble = true;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}


void MainWindow::on_pushButton_exponent_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    if (exponentTrigger && isNumberIsDouble) {
        expression = expression + button->text();

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = true;
        exponentTrigger = false;
        nullTrigger = false;

        isNumberIsDouble = true;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}

void MainWindow::on_pushButton_negative_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());
    if (negativeSymbolTrigger &&
            (expression.right(3) == " - " || expression.right(3) == " + "
            || expression.right(3) == " * " || expression.right(3) == " / ")) {
        expression = expression + "(-";
        numBrackets += 1;

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = true;
    } else if (expression.size() > 0 && (expression.right(1) == "e" || expression.right(1) == "E") && negativeSymbolTrigger) {
        expression = expression + "-";

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = false;
    } else if (negativeSymbolTrigger) {
        expression = expression + "-";

        numberTrigger = true;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = false;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = true;
    }

    ui->result_show->setText(expression);
    SendToServer(button);
}


void MainWindow::on_pushButton_clear_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    expression = "";
    numBrackets = 0;


    numberTrigger = true;
    leftBracketTrigger = true;
    rightBracketTrigger = false;
    mathOperationTrigger = false;
    dotTrigger = false;
    negativeSymbolTrigger = true;
    exponentTrigger = false;


    nullTrigger = true;

    isNumberIsDouble = false;

    ui->result_show->setText(expression);
    SendToServer(button);
}

// ОТПРАВКА НА СЕРВЕР НЕ ТАКАЯ КАКАЯ ДОЛЖНА БЫТЬ
// И ФУНКЦИЯ РАБОТАЕТ НЕ ОЛЖНЫМ ОБРАЗОМ
void MainWindow::on_pushButton_equal_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());
    nullTrigger = true;

    if (numBrackets == 0) {
        SendToServer(button, true);

        numberTrigger = false;
        leftBracketTrigger = false;
        rightBracketTrigger = false;
        mathOperationTrigger = true;
        dotTrigger = false;
        negativeSymbolTrigger = false;
        exponentTrigger = false;
        nullTrigger = false;
    } else {
        SendToServer(button);
    }

}


void MainWindow::on_pushButton_insert_history_result_clicked()
{
    QPushButton *button = static_cast<QPushButton *>(sender());

    QAbstractItemModel* model = ui->tableView->model();


    QModelIndexList selectedIndex = ui->tableView->selectionModel()->selectedIndexes();

    if (!selectedIndex.isEmpty()) {
        QModelIndex firstIndex = selectedIndex.first();
        int row = firstIndex.row();

        QModelIndex index = model->index(row, 4);
        QVariant data = model->data(index);
        expression = expression + data.toString();
        ui->result_show->setText(expression);
    }

    SendToServer(button);

    numberTrigger = false;
    leftBracketTrigger = false;
    rightBracketTrigger = true;
    mathOperationTrigger = true;
    dotTrigger = false;
    negativeSymbolTrigger = false;
    exponentTrigger = false;
    nullTrigger = false;



    //qDebug() << data.toString();
    //qDebug() << "...";

}



// ОТПРАВКА НА СЕРВЕР НЕ ТАКАЯ КАКАЯ ДОЛЖНА БЫТЬ
// И ФУНКЦИЯ РАБОТАЕТ НЕ ОЛЖНЫМ ОБРАЗОМ
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_0){  
       //if (expression.size() > 0 && !isNumberIsDouble && (expression.right(1).at(0).isDigit() && expression.right(1) != "0")) {
        if (expression.size() > 0 && !isNumberIsDouble && (expression.right(1).at(0).isDigit())) {
            expression += button->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = false;
        } else if (isNumberIsDouble && nullTrigger) {
            expression += button->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (nullTrigger) {
            expression += button->text();

            numberTrigger = false;
            leftBracketTrigger = false;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = false;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_1){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_2){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_3){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_4){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_5){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_6){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_7){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_8){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_9){
        if (numberTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = true;
            nullTrigger = true;
        } else if (numberTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = true;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }

    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
        nullTrigger = true;

        if (numBrackets == 0) {
            SendToServer(button, true);

            numberTrigger = false;
            leftBracketTrigger = false;
            rightBracketTrigger = false;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = false;
        } else {
            SendToServer(e);
        }
    }
    if (e->key() == Qt::Key_Minus) {
        if (mathOperationTrigger) {
            expression = expression + " " + e->text() + " ";

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }
        if (expression.size() > 0 && mathOperationTrigger == false) {
            if (expression.right(3) == " - " || expression.right(3) == " + "
                    || expression.right(3) == " * " || expression.right(3) == " / ") {
                expression.chop(3);

                expression = expression + " " + e->text() + " ";

                numberTrigger = true;
                leftBracketTrigger = true;
                rightBracketTrigger = false;
                mathOperationTrigger = false;
                dotTrigger = false;
                negativeSymbolTrigger = true;
                exponentTrigger = false;
                nullTrigger = true;

                isNumberIsDouble = false;
            }
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_Plus) {
        if (mathOperationTrigger) {
            expression = expression + " " + e->text() + " ";

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }
        if (expression.size() > 0 && mathOperationTrigger == false) {
            if (expression.right(3) == " - " || expression.right(3) == " + "
                    || expression.right(3) == " * " || expression.right(3) == " / ") {
                expression.chop(3);

                expression = expression + " " + e->text() + " ";

                numberTrigger = true;
                leftBracketTrigger = true;
                rightBracketTrigger = false;
                mathOperationTrigger = false;
                dotTrigger = false;
                negativeSymbolTrigger = true;
                exponentTrigger = false;
                nullTrigger = true;

                isNumberIsDouble = false;
            }
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_Asterisk) {
        if (mathOperationTrigger) {
            expression = expression + " " + e->text() + " ";

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }
        if (expression.size() > 0 && mathOperationTrigger == false) {
            if (expression.right(3) == " - " || expression.right(3) == " + "
                    || expression.right(3) == " * " || expression.right(3) == " / ") {
                expression.chop(3);

                expression = expression + " " + e->text() + " ";

                numberTrigger = true;
                leftBracketTrigger = true;
                rightBracketTrigger = false;
                mathOperationTrigger = false;
                dotTrigger = false;
                negativeSymbolTrigger = true;
                exponentTrigger = false;
                nullTrigger = true;

                isNumberIsDouble = false;
            }
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_Slash) {
        if (mathOperationTrigger) {
            expression = expression + " " + e->text() + " ";

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }
        if (expression.size() > 0 && mathOperationTrigger == false) {
            if (expression.right(3) == " - " || expression.right(3) == " + "
                    || expression.right(3) == " * " || expression.right(3) == " / ") {
                expression.chop(3);

                expression = expression + " " + e->text() + " ";

                numberTrigger = true;
                leftBracketTrigger = true;
                rightBracketTrigger = false;
                mathOperationTrigger = false;
                dotTrigger = false;
                negativeSymbolTrigger = true;
                exponentTrigger = false;
                nullTrigger = true;

                isNumberIsDouble = false;
            }
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }

    if (e->key() == Qt::Key_ParenLeft) {
        if (leftBracketTrigger) {
            expression = expression + e->text();
            numBrackets += 1;

            numberTrigger = true;
            leftBracketTrigger = true;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = false;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_ParenRight) {
        if (numBrackets > 0 && rightBracketTrigger) {
            expression = expression + e->text();

            numBrackets -= 1;

            numberTrigger = false;
            leftBracketTrigger = false;
            rightBracketTrigger = true;
            mathOperationTrigger = true;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = false;

            isNumberIsDouble = false;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_E) {
        if (exponentTrigger && isNumberIsDouble) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = true;
            exponentTrigger = false;
            nullTrigger = false;

            isNumberIsDouble = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
    if (e->key() == Qt::Key_Period || e->key() == Qt::Key_periodcentered) {
        if (!isNumberIsDouble && dotTrigger) {
            expression = expression + e->text();

            numberTrigger = true;
            leftBracketTrigger = false;
            rightBracketTrigger = false;
            mathOperationTrigger = false;
            dotTrigger = false;
            negativeSymbolTrigger = false;
            exponentTrigger = false;
            nullTrigger = true;

            isNumberIsDouble = true;
        }

        SendToServer(e);
        ui->result_show->setText(expression);
    }
}




