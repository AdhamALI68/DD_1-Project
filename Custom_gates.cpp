#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

class LogicalExpressionEvaluator {
private:
    bool isOperator(char c) {
        return (c == '&' || c == '|' || c == '~' || c == '(' || c == ')');
    }

    int precedence(char c) {
        switch (c) {
            case '|':
                return 1;
            case '&':
                return 2; // Adjusted for clarity, though not necessary
            case '~':
                return 3; // Highest precedence for unary operator
            case '(':
            case ')':
                return 0; // Parentheses have highest precedence
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

    // Function to extract the next operand (variable) from the expression
    string extractOperand(string& expression, int& pos) {
        string operand = "";
        while (pos < expression.length() && !isOperator(expression[pos])) {
            operand += expression[pos];
            pos++;
        }
        return operand;
    }

public:
    bool evaluateInfixExpression(string expression, unordered_map<string, bool>& variables) {
        stack<bool> operands;
        stack<char> operators;
        int i = 0;

        while (i < expression.length()) {
            char c = expression[i];
            if (!isOperator(c)) {
                string operand = extractOperand(expression, i);
                cout<<"operand "<<operand<<"\n";
                if (variables.find(operand) != variables.end()) {
                    operands.push(variables[operand]);
                } else {
                    throw "Undefined variable: " + operand;
                }
                continue; // Skip to the next iteration
            }
            if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (operators.top() != '(') {
                    if (operators.top() == '~') {
                        bool a = operands.top();
                        operands.pop();
                        operands.push(evaluate(a, operators.top()));
                    } else {
                        bool b = operands.top();
                        operands.pop();
                        bool a = operands.top();
                        operands.pop();
                        operands.push(evaluate(a, b, operators.top()));
                    }
                    operators.pop();
                }
                operators.pop(); // Pop '('
            } else if (isOperator(c)) {
                while (!operators.empty() && precedence(c) <= precedence(operators.top())) {
                    if (operators.top() == '~') {
                        bool a = operands.top();
                        operands.pop();
                        operands.push(evaluate(a, operators.top()));
                    } else {
                        bool b = operands.top();
                        operands.pop();
                        bool a = operands.top();
                        operands.pop();
                        operands.push(evaluate(a, b, operators.top()));
                    }
                    operators.pop();
                }
                operators.push(c);
            }
            i++;
        }

        while (!operators.empty()) {
            if (operators.top() == '~') {
                bool a = operands.top();
                operands.pop();
                operands.push(evaluate(a, operators.top()));
            } else {
                bool b = operands.top();
                operands.pop();
                bool a = operands.top();
                operands.pop();
                operands.push(evaluate(a, b, operators.top()));
            }
            operators.pop();
        }

        return operands.top();
    }
};

int main() {
    LogicalExpressionEvaluator evaluator;
    unordered_map<string, bool> variables = {
        {"in0", true},
        {"in1", false},
        {"in2", true},
        {"in3", true}
    };
    string expression = "(in3|(in0&in1))&(in0&in2)";
    try {
        bool result = evaluator.evaluateInfixExpression(expression, variables);
        cout << "Result: " << (result ? "true" : "false") << endl;
    } catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }

    return 0;
}
