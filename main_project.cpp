#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <stack>
#include <string>
#include <exception>


using namespace std;
bool compareTuples(tuple<int, string, int> &a, tuple<int, string, int> &b) {
    return get<0>(a) < get<0>(b);
}

template <typename T>
void printer(vector<T> &f){
for (T i: f){
cout << i << endl;
}
}

template <typename Z>
void map_printer(unordered_map<string, Z> m){
for (auto& it: m) {
    cout << it.first << "   " <<endl;
}
}


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
    bool evaluateInfixExpression(string expression, unordered_map<string, bool>& variables , unordered_map<string, int>& input_map) {
        stack<bool> operands;
        stack<char> operators;
        int i = 0;
        
        while (i < expression.length()) {
            char c = expression[i];
            if (!isOperator(c)) {
                string operand = extractOperand(expression, i);
                        cout<<operand;
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
                operators.pop();
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

using LogicFunction = function<bool(const vector<bool>&, int)>;


vector<tuple<int, char, int>> readFromFile(const string& filename) {
    vector<tuple<int, char, int>> readings;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        int value1;
        char value2;
        int value3;

        // Read comma-separated values from the line
        if (getline(ss, token, ','))
            value1 = stoi(token);
        if (getline(ss, token, ','))
            value2 = token[1]; // Extracting the first character of the string
        if (getline(ss, token, ','))
            value3 = stoi(token);

        // Store the values in a tuple and push it into the vector
        readings.push_back(make_tuple(value1, value2, value3));
    }

    return readings;
}


struct Component {
    int num_inputs;
    int delay_ps;
    string logic;
};

// Define a map to store components
unordered_map<string, Component> component_library;

// Load library file and store propagation delay and number of inputs
void loadLibrary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open library file: " << filename << endl;
        exit(1);
    }
    string line;
    while (getline(file, line)) {
        // Remove spaces that are not after a comma
        bool afterComma = false;
        line.erase(remove_if(line.begin(), line.end(), [&afterComma](char c) {
            if (c == ',') {
                afterComma = true;
                return false;
            }
            if (c == ' ' && !afterComma) {
                return true;
            }
            if (c != ' ') {
                afterComma = false;
            }
            return false;
        }), line.end());

        // Now process the modified line
        stringstream ss(line);
        string firstWord, expression, lastWord, word;
        vector<string> words;

        // Extract the first word
        // Extract the last word
        while (ss >> word) {
            words.push_back(word); // Overwrite lastWord until the last word is reached
        }
        firstWord = words[0];
        expression = words[2];
        lastWord = words[words.size() - 1];
        expression.pop_back();
        // cout << "first " << firstWord << " expression " << expression <<"last word " <<lastWord << endl;

        int lastInteger = std::stoi(lastWord);
        // Assuming component_library is declared as std::map<std::string, std::tuple<char, int, std::string>> component_library;
        component_library[firstWord] = {firstWord[0], lastInteger, expression};
    }
    file.close();
    }

void parse_cir_file(const string& filename, vector<string>& inputs, vector<vector<string>>& components, vector<vector<string>>& ins) {
    int i=0;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open input file: " << filename << endl;
        exit(1);
    }
    string line;
    bool reading_inputs = false;
    bool reading_components = false;
    while (getline(file, line))
    {
        if (line == "INPUTS:") {
            reading_inputs = true;
            continue;
        }
        if (line == "COMPONENTS:") {
            reading_inputs = false;
            reading_components = true;
            continue;
        }
        if (reading_inputs && !line.empty()) {
            inputs.push_back(line);
        }
        if (reading_components && !line.empty()) 
        {
            stringstream ss(line);
            vector<string> component;
            string token;
            vector<string>vv;
            while (ss >> token) 
            {
                i++;
                component.push_back(token);

                if(i>=4){vv.push_back(token);

            }
        }
            ins.push_back(vv);
            vv.clear();
            i=0;
            
            components.push_back(component);
        }
        for (int i=0; i<ins.size(); i++) {
            for(int j=0; j<ins[i].size();j++)
            {
                if(ins[i][j][ins[i][j].length()-1]==','){ins[i][j].pop_back();}
                if(ins[i][j][ins[i][j].length()-1]==' '){ins[i][j].pop_back();}
                if(ins[i][j][ins[i][j].length()-1]==','){ins[i][j].pop_back();}

            }
        }
    }
    file.close();
}


void funccall(vector<tuple<string,string,vector<bool>>> vec, unordered_map<string, int> &input_map, vector<vector<string>> ins, int sd,unordered_map<string, int> delay_map,int delay, std::ofstream& outputFile,    vector <tuple <int ,string, int>> &outs) {

    for (int i = 0; i < vec.size(); i++) {
        string gatename = std::get<0>(vec[i]);
        string output = std::get<1>(vec[i]);
        vector<bool> inp;
        for (int j = 0;  j < ins[i].size(); j++) {
            inp.push_back(input_map[ins[i][j]]);
        }
        string z = gatename + ',';
        LogicalExpressionEvaluator evaluator;
        unordered_map<string, bool> variables;
        string to_insert = component_library[z].logic;
        string temp = "";
        int x = 0;
        bool flag = false;
        vector <int> nums;
        for (int k =0 ; k < to_insert.length(); k++){
            if ((to_insert[k] >= 48 && to_insert[k] <= 57)){
                temp += to_insert[k];
                flag = true;
            }
            if (flag == true && (to_insert[k] >57 || to_insert[k] < 48)){
                nums.push_back(atoi(temp.c_str()));
                temp = "";
                flag = false;
            }
        }
        for (int h : nums){
            x=max(x,h);
        }

        for (int k = 0; k< ins[i].size(); k++)
        {
            string z="i";
            z+=to_string(k+1);
            variables[z]=input_map[ins[i][k]];
        }
        if(sd==0){
            cout<<output<<"   "<<component_library[z].logic<<"\n";
            input_map[output]=evaluator.evaluateInfixExpression(component_library[z].logic, variables,input_map);
                            outputFile << delay_map[output]+delay << ", " << output << ", " << input_map[output] << "\n";

             outs.push_back({delay_map[output]+delay , output , input_map[output]});
        } else {
            int zin=input_map[output];
            input_map[output]=evaluator.evaluateInfixExpression(component_library[z].logic, variables,input_map);
            if(zin!=input_map[output]){
                outputFile << delay_map[output]+delay << ", " << output << ", " << input_map[output] << "\n";
               outs.push_back(make_tuple(delay_map[output]+delay , output , input_map[output]));
            }
        }
    }
}

vector<vector<tuple<bool,string>>> violation_check(unordered_map<string, Component> component_library , unordered_map<string, int> component_circuit , vector<vector<string>> components, vector<string> inputs){
vector <string> gates_lib;
vector <string> output_gates;
vector <vector<tuple<bool,string>>> res;
vector<tuple<bool, string>> t;
bool flag,bool2,pushed = 0;
string invalid_in_out = "\0";

for (auto &x: component_library){
    gates_lib.push_back(x.first);
}
for (auto &y: component_circuit){
    flag = 0;
    for (auto &z: gates_lib){
        if (y.first == z){
            flag = 1;
            break;
        }
    }
    if (flag == 0){t.push_back({1,y.first}); res.push_back(t); t.clear(); pushed = true;}
}
if (pushed == false){t.clear(); t.push_back({0,"\0"}); res.push_back(t);}
for (auto&a : components){
    for (int i = 0; i < a.size(); i++){
        if((i+1)%3 == 0){output_gates.push_back(a[i]);}
    }
}
for (auto &in: inputs){
    if (bool2 == true){break;}
    for (auto &z: output_gates){
        if (in == z.substr(0,z.length()-1)){
            // cout << "in   " << in << "      z" << z <<" value:" << (in == z.substr(0,z.length()-1)); 
            bool2 = true;
            invalid_in_out = in;
            break;
        }
    }
}
t.clear();
t.push_back({bool2, invalid_in_out});
res.push_back(t);
return res;
}

class ERROR_IN_OUT: public logic_error{
    public:
    ERROR_IN_OUT(string x): logic_error(x){}
};

class ERROR_IN_GATE_NAME : public logic_error{
    public:
    ERROR_IN_GATE_NAME(string x): logic_error(x){}
};


int main() {
    
    unordered_map<string, bool> variables = {
        {"in0", true},
        {"in1", false},
        {"in2", true},
        {"in3", true}
    };

    loadLibrary("tests/3.lib");
    vector<tuple<string,string,vector<bool>>>vec;

    vector<string> inputs;
    vector<vector<string>> components;
    vector<vector<string>> ins;

    parse_cir_file("tests/3.cir", inputs, components,ins);
    // Store inputs in a map and initialize to zero
    unordered_map<string, int> input_map;
    for (const string& input : inputs) {
        input_map[input] = 0;
    }

    // Store component functions and number of parameters
    unordered_map<string, int> component_functions;
    for (const auto& component : components) {
        string function_name = component[1]; // Name of the function
        int num_parameters = component.size() - 3; // Number of parameters (excluding name, output, and comma)
        component_functions[function_name] = num_parameters;
    }

    //checking for compatiblities between lib and circ files
    try{
        vector<vector<tuple<bool,string>>> res = violation_check(component_library , component_functions, components , inputs);
        if (get<0>(res[0][0]) == 1){
            throw ERROR_IN_GATE_NAME(get<1>(res[0][0]));
        }
        if (get<0>(res[1][0]) == 1){
            throw ERROR_IN_OUT(get<1>(res[1][0]));
        }
    //Example usage:
    //Printing the input map
    unordered_map<string, int> operation;
    operation=component_functions;

    unordered_map<string, int> delay_map; // Map to store propagation delay for each wire

    for (int i = 0; i < components.size(); i++)
    {
        string output="";
        string gatename="";
        vector<string>in;
        vector<bool>inp;
        int max_delay = 0; // Initialize the maximum delay for this component
        for (int j = 0; j <components[i].size(); j++)
        {
            if(operation[components[i][j]]!=0){
                if(gatename==""){gatename=components[i][j]; gatename.pop_back(); }
                output=components[i][j+1];          
                j++; continue; }

            if(output!=""){
                if(components[i][j][components[i][j].length()-1]==','){components[i][j].erase(components[i][j].length()-1);}
                in.push_back(components[i][j]);

            }
        }
        for (int k = 0; k < in.size(); k++)
        {
            // inp.push_back(input_map[in[k]]);
            // Update the maximum delay for this component based on the input wire delays
            if (delay_map.find(in[k]) != delay_map.end()) {
                max_delay = max(max_delay, delay_map[in[k]]);
            }
        }
        string z=gatename+',';
        output.pop_back();
        vec.push_back({gatename,output,inp});
        delay_map[output] = max_delay+ component_library[z].delay_ps;
        in.clear();

    }
    vector <tuple <int ,string, int>> outs; 

    cout << "Propagation Delays:" << endl;

    vector<tuple<int, char, int>> readings = readFromFile("tests/3.stim");
    tuple<int, char, int>v{0,'A',0};
    readings.insert(readings.begin(),v);
    // Printing out the vector to verify the results
    int sd=0;
        std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    for (const auto& reading : readings) 
    {
        string c="";
        int delay=get<0>(reading);
        c+=get<1>(reading);
        input_map[c]=get<2>(reading);
        vector<bool>vi;
            outputFile << delay_map[c]+delay << ", " << c << ", " << input_map[c] << "\n";
            outs.push_back({delay_map[c]+delay,c,input_map[c]});

        for (const auto& x: input_map) 
        {

            if(x.first.length()==1){
                vi.push_back(x.second);}

        }
        funccall(vec, input_map, ins,sd++,delay_map,delay,outputFile,outs);

    }
    sort(outs.begin(), outs.end(), compareTuples);
    cout << get<0>(outs[0]) << endl;
    ofstream o;
    o.open("output_final.txt");
    outs.erase(outs.begin(),outs.begin()+1);
    //vector <>
    for (int i = 0; i < outs.size();i++){
        o << get<0>(outs[i])<<", " <<get<1>(outs[i]) << ", " << get<2>(outs[i]) << endl;
    }

    //for ()
    o.close();}

    catch(const ERROR_IN_GATE_NAME &x){
        cout <<"Incompatibilites between lib and circ files have been found; " <<x.what() << " is declared in the circ file but not in the lib file.\n";
    }
    catch(const ERROR_IN_OUT &y){
       cout << "Input " << y.what() << " has been found as an output, which is invalid !!";
    }
}