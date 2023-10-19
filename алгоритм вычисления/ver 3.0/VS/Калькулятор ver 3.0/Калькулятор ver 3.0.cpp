#include <iostream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; // Приоритет операции возведения в степень
    return 0;
}

double applyOperator(char op, double operand1, double operand2) {
    switch (op) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/': return operand1 / operand2;
    case '^': return pow(operand1, operand2);
    default: return 0.0;
    }
}

double calculateExpression(const string& expression) {
    stack<double> values;
    stack<char> operators;
    int i = 0;

    while (i < expression.length()) {
        if (expression[i] == ' ') {
            i++;
            continue;
        }

        if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
            string numStr;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.' || expression[i] == 'e' || expression[i] == 'E' || expression[i] == '+' || expression[i] == '-')) {
                numStr += expression[i++];
            }
            double num = stod(numStr);
            values.push(num);
        }
        else if (expression[i] == '(') {
            operators.push(expression[i]);
            i++;
        }
        else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
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
                char op = operators.top();
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
        char op = operators.top();
        operators.pop();
        double operand2 = values.top();
        values.pop();
        double operand1 = values.top();
        values.pop();
        values.push(applyOperator(op, operand1, operand2));
    }

    return values.top();
}

int main() {
    string expression;
    cout << "Введите выражение: ";
    getline(cin, expression);

    double result = calculateExpression(expression);
    cout << "Результат: " << result << endl;

    return 0;
}


//----------------------------------------------------------//
// Пример выражения: 2^3 + 5 * (2.5e2 - 1.2E-1)
//----------------------------------------------------------//
