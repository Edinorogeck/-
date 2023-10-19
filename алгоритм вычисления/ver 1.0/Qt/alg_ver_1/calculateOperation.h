#ifndef CALCULATEOPERATION_H
#define CALCULATEOPERATION_H

#include <QString>

#include <QStack>
#include <QtMath>
#include <iostream>
#include <sstream>
#include <string>

#include <QDebug>

struct Leksema //Структура, описывающая любое число или операцию
{
    char type; // 0 для чисел, "+" для операции сложения и т.д.
    double value; //Значение (только для чисел). У операций значение всегда "0"
};

bool Maths(QStack <Leksema>& Stack_n, QStack <Leksema>& Stack_o, Leksema& item);
int getRang(char Ch);
QString calculateOperation (QString qstr);




#endif // CALCULATEOPERATION_H
