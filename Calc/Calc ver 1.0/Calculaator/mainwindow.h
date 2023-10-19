#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

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

private slots:
    void digits_numbers();
    void on_pushButton_dot_clicked();
    void math_operations();
    void on_pushButton_equal_clicked();  // =
    void on_pushButton_clear_clicked();

    //void operations();


    //void on_pushButton_divide_clicked(); // /
    //void on_pushButton_minus_clicked();  // -
    //void on_pushButton_multi_clicked();  // x
    //void on_pushButton_plus_clicked();   // +
    //void on_pushButton_clear_clicked();  // AC

};
#endif // MAINWINDOW_H
