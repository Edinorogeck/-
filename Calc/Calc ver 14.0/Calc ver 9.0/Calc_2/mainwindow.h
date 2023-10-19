#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include <QTcpSocket>
#include <QStandardItemModel>

#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *e) override;


private:
    Ui::MainWindow *ui;
    QStandardItemModel* logTableModel;


    // Для логов //
    QTcpSocket *socket;                         // Client items
    QByteArray Data;                            // Client items
    quint16 nextBlockSize;                      // Client items
                                                // Client items
    void connectToHost();                       // Client items
    //void SendToServer(QString str);             // Client items
    void SendToServer(QPushButton *klick, bool calculate);      // Client items
    void SendToServer(QKeyEvent *klick, bool calculate);        // Client items
    //-----------//

    // Для истории вычислений //
    QList<QList<QStandardItem*>> rowList; // Объявление списка строк таблицы
    //QList<QStandardItem*> row; // Объявление списка значений столбцов таблицытаблицы
    //------------------------//

    // Обработчик полученного сообщения
    void MessageHandler(QString jsonInString);
    // Метод для обновления таблицы истории
    void RefreshHistoryTable();
    void MakeHistoryTable();

public slots:
    // Для логов //
    void slotReadyRead();
    //-----------//

private slots:
    void digits_numbers();
    void math_operations();

    void on_pushButton_0_clicked();
    void on_pushButton_equal_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_left_bracket_clicked();
    void on_pushButton_right_bracket_clicked();
    void on_pushButton_dot_clicked();
//    void on_pushButton_clicked();
    void on_pushButton_negative_clicked();
    void on_pushButton_exponent_clicked();
    void on_pushButton_insert_history_result_clicked();
};
#endif // MAINWINDOW_H
