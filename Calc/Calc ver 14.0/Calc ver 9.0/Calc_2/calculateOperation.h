#ifndef CALCULATEOPERATION_H
#define CALCULATEOPERATION_H

#include <QString>
#include <QChar>

#include <stack>
#include <cmath>

using namespace std;

int getPriority(QChar op);

double applyOperator(QChar op, double operand1, double operand2);

QString calculateOperation(const QString & expression);


#endif // CALCULATEOPERATION_H
