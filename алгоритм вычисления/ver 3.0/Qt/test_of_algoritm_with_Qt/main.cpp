#include <QCoreApplication>
#include <QDebug>
#include <calculateOperation.h>



int main(int argc, char *argv[])

{
    QCoreApplication a(argc, argv);

    QString expression = "2^3 + 5 * (2.5e2 - 1.2E-1)";
    //QString expression = "48 + (-56 / 8)";

    QString result = calculateOperation(expression);

    qDebug() << result;
    return a.exec();
}
