#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// Forward declarations
class LogicGate;
class AndGate;
class OrGate;
class NotGate;
class InputGate;

// Base class for all logic gates
class LogicGate {
public:
    virtual bool evaluate(unordered_map<string, bool>& inputValues) = 0;
};

// AND gate implementation
class AndGate : public LogicGate {
private:
    LogicGate* left;
    LogicGate* right;
public:
    AndGate(LogicGate* l, LogicGate* r) : left(l), right(r) {}

    bool evaluate(unordered_map<string, bool>& inputValues) override {
        return left->evaluate(inputValues) && right->evaluate(inputValues);
    }
};

// OR gate implementation
class OrGate : public LogicGate {
private:
    LogicGate* left;
    LogicGate* right;
public:
    OrGate(LogicGate* l, LogicGate* r) : left(l), right(r) {}

    bool evaluate(unordered_map<string, bool>& inputValues) override {
        return left->evaluate(inputValues) || right->evaluate(inputValues);
    }
};

// NOT gate implementation
class NotGate : public LogicGate {
private:
    LogicGate* operand;
public:
    NotGate(LogicGate* op) : operand(op) {}

    bool evaluate(unordered_map<string, bool>& inputValues) override {
        return !operand->evaluate(inputValues);
    }
};

// Input gate implementation
class InputGate : public LogicGate {
private:
    string name;
public:
    InputGate(string n) : name(n) {}

    bool evaluate(unordered_map<string, bool>& inputValues) override {
        return inputValues[name];
    }
};

// Parser class
class Parser {
private:
    string expression;
    size_t index;
public:
    Parser(string expr) : expression(expr), index(0) {}

    LogicGate* parse() {
        return parseExpression();
    }

    LogicGate* parseExpression() {
        char token = expression[index++];
        if (token == '(') {
            LogicGate* gate = parseGate();
            index++; // Consume ')'
            return gate;
        }
        else if (token == '~') {
            return new NotGate(parseExpression());
        }
        else {
            return new InputGate(string(1, token));
        }
    }

    LogicGate* parseGate() {
        string gateType;
        while (expression[index] != ' ') {
            gateType += expression[index++];
        }
        index++; // Consume space
        LogicGate* left = parseExpression();
        LogicGate* right = parseExpression();
        if (gateType == "and") {
            return new AndGate(left, right);
        }
        else if (gateType == "or") {
            return new OrGate(left, right);
        }
        else {
            throw invalid_argument("Unknown gate type: " + gateType);
        }
    }
};

int main() {
    string expression = "(in0 | in1) & (~in0 | ~in1)";
    Parser parser(expression);
    LogicGate* gate = parser.parse();

    unordered_map<string, bool> inputValues = {{"in0", false}, {"in1", true}};
    bool result = gate->evaluate(inputValues);
    cout << "Result: " << result << endl;

    // Free memory
    delete gate;

    return 0;
}
