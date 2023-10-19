#include <calculateOperation.h>


int getPriority(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; // Приоритет операции возведения в степень
    return 0;
}

double applyOperator(QChar op, double operand1, double operand2) {
    switch (op.toLatin1()) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/': return operand1 / operand2;
    case '^': return pow(operand1, operand2);
    default: return 0.0;
    }
}

QString calculateOperation(const QString & expression) {
    stack<double> values;
    stack<QChar> operators;
    //long unsigned int i = 0;
    int i = 0;

    while (i < expression.length()) {
        if (expression[i] == ' ') {
            i++;
            continue;
        }

        if (expression[i].isDigit() || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
            QString numStr;
            while (i < expression.length() && (expression[i].isDigit() || expression[i] == '.' || expression[i] == 'e' || expression[i] == 'E' || expression[i] == '+' || expression[i] == '-')) {
                numStr += expression[i++];
            }
            double num = numStr.toDouble();
            values.push(num);
        }
        else if (expression[i] == '(') {
            operators.push(expression[i]);
            i++;
        }
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                QChar op = operators.top();
                operators.pop();
                double operand2 = values.top();
                values.pop();
                double operand1 = values.top();
                values.pop();
                values.push(applyOperator(op, operand1, operand2));
            }
            operators.pop();
            i++;
        }
        else {
            while (!operators.empty() && getPriority(operators.top()) >= getPriority(expression[i])) {
                QChar op = operators.top();
                operators.pop();
                double operand2 = values.top();
                values.pop();
                double operand1 = values.top();
                values.pop();
                values.push(applyOperator(op, operand1, operand2));
            }
            operators.push(expression[i]);
            i++;
        }
    }

    while (!operators.empty()) {
        QChar op = operators.top();
        operators.pop();
        double operand2 = values.top();
        values.pop();
        double operand1 = values.top();
        values.pop();
        values.push(applyOperator(op, operand1, operand2));
    }

    return QString::number(values.top());
}

