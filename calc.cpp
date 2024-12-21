#include <iostream>
#include <stack>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                throw invalid_argument("Деление на ноль");
            }
            return a / b;
    }
    return 0;
}

double evaluate(const string& expression) {
    stack<double> values;  
    stack<char> ops;     
    int i = 0;
    while (i < expression.size()) {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        if (isdigit(expression[i])) {
            stringstream ss;
            while (i < expression.size() && (isdigit(expression[i]) || expression[i] == '.')) {
                ss << expression[i++];
            }
            double num;
            ss >> num;
            values.push(num);
        }

        else if (expression[i] == '(') {
            ops.push(expression[i++]);
        }

        else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }

            if (ops.empty() || ops.top() != '(') {
                throw invalid_argument("Лишние скобки");
            }

            ops.pop();
            i++;
        }

        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            ops.push(expression[i++]);
        }

        else {
            throw invalid_argument("Недопустимый символ");
        }
    }

    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    if (values.size() != 1) {
        throw invalid_argument("Неправильно введенное выражение");
    }

    return values.top();
}



int main() {
    string expression;
    cout << "Введите математическое выражение: ";
    getline(cin, expression);

    try {
        double res = evaluate(expression);
        cout << "Результат: " << res << endl;
    } catch (const exception& el) {
        cerr << "Ошибка: " << el.what() << endl;
    }

    return 0;
}
