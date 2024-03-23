#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

class LogicalExpressionEvaluator {
private:
    bool isOperator(char c) {
        return (c == '&' || c == '|' || c == '~' );
    }

    int precedence(char c) {
        switch (c) {
            case '|':
                return 1;
            case '&':
                return 2; // Adjusted for clarity, though not necessary
            case '~':
                return 3; // Highest precedence for unary operator
        }
        return -1;
    }

    // Overloaded evaluate function for unary operators
    bool evaluate(bool a, char op) {
        switch (op) {
            case '~':
                return !a;
        }
        return false; // Should never reach here
    }

    // Existing evaluate function for binary operators
    bool evaluate(bool a, bool b, char op) {
        switch (op) {
            case '&':
                return a && b;
            case '|':
                return a || b;
        }
        return false; // Should never reach here
    }

public:
    bool evaluateInfixExpression(string expression, unordered_map<char, bool>& variables) {
        stack<bool> operands;
        stack<char> operators;
        int i = 0;

        while (i < expression.length()) {
            char c = expression[i];
            if (isalpha(c)) {
                operands.push(variables[c]);
            } else if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (operators.top() != '(') {
                    if (operators.top() == '~') {
                        bool a = operands.top();
                        operands.pop();
                        char op = operators.top();
                        operators.pop();
                        operands.push(evaluate(a, op));
                    } else {
                        bool b = operands.top();
                        operands.pop();
                        bool a = operands.top();
                        operands.pop();
                        char op = operators.top();
                        operators.pop();
                        operands.push(evaluate(a, b, op));
                    }
                }
                operators.pop();
            } else if (isOperator(c)) {
                while (!operators.empty() && precedence(c) <= precedence(operators.top())) {
                    if (operators.top() == '~') {
                        bool a = operands.top();
                        operands.pop();
                        char op = operators.top();
                        operators.pop();
                        operands.push(evaluate(a, op));
                    } else {
                        bool b = operands.top();
                        operands.pop();
                        bool a = operands.top();
                        operands.pop();
                        char op = operators.top();
                        operators.pop();
                        operands.push(evaluate(a, b, op));
                    }
                }
                operators.push(c);
            }
            i++;
        }

        while (!operators.empty()) {
            if (operators.top() == '~') {
                bool a = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                operands.push(evaluate(a, op));
            } else {
                bool b = operands.top();
                operands.pop();
                bool a = operands.top();
                operands.pop();
                char op = operators.top();
                operators.pop();
                operands.push(evaluate(a, b, op));
            }
        }

        return operands.top();
    }
};

int main() {
    LogicalExpressionEvaluator evaluator;
    unordered_map<char, bool> variables = {
        {'a', true},
        {'b', false},
        {'c', true},
        {'d',true}
    };
    string expression = "(~d|(a&b))&(a&c)";
    try {
        bool result = evaluator.evaluateInfixExpression(expression, variables);
        cout << "Result: " << (result ? "true" : "false") << endl;
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }

    return 0;
}
