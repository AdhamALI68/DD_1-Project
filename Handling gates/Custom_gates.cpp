#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

bool logic_AND(const vector<bool>& inputs, int delay) {
    bool result = true;
    for (bool input : inputs) {
        result = result && input;
    }
    // Modify delay if needed
    return result;
}

bool logic_OR(const vector<bool>& inputs, int delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result || input;
    }
    // Modify delay if needed
    return result;
}

bool logic_NAND(const vector<bool>& inputs, int delay) {
    bool result = !(logic_AND(inputs, delay));
    // Modify delay if needed
    for (int i=0; i<inputs.size();i++) {
        //cout<<"ber"<<inputs[i]<<"bet§"<<"\n";
    }
    //cout<<" nsnd size "<<inputs.size();
    //cout<<inputs[1];    return result;
}

bool logic_XOR(const vector<bool>& inputs, int delay) {

    bool result = false;
    for (bool input : inputs) {
        result = result ^ input;
        //cout<<input<<" "<<"sddd"<<"\n";
    }
    //cout<<result;
    
    // Modify delay if needed
    return result;
}

bool logic_NOT(const vector<bool>& inputs, int delay) {
    vector<bool> inverted_inputs = inputs; // Copy inputs to avoid modifying original vector
    for (int i = 0; i < inputs.size(); ++i) {
        inverted_inputs[i] = !inverted_inputs[i]; // Invert each input
    }
    // Modify delay if needed
    return inverted_inputs[0]; // NOT gate returns inversion of the input
}


template <typename T>
void printer(vector<T> &f){
for (T i: f){
cout << i << endl;
}
}



vector<string> tokenizer(string base){
    vector <string> individual_gates_with_operations;
    string temp;
    string converter; 
    int flag = 0; 
    
    for (char i : base){
        if (flag == 0 && ( (i== '!') || (i == '&') || (i == '|') || (i == '~'))){
            converter = i;
            individual_gates_with_operations.push_back(converter);
        }
        if(i == '('){
            flag = 1;
        }
        if (flag == 1){
            temp+=i;
        }
        if (i == ')'){
            flag = 0;
            individual_gates_with_operations.push_back(temp);
            temp = "";
        }
    }
    return individual_gates_with_operations;
}

int count_inputs(string expression){
int inputs_num = 0;
for (char i : expression){
    if (i == '&' || i == '|' || i == '~'){
        inputs_num ++;
    }
}
return inputs_num+1;
}

vector <string> space_tokenizer(string s){
vector <string> tokens;
tokens.clear();
     
    // stringstream class check1
    stringstream check1(s);
     
    string intermediate;
     
    // Tokenizing w.r.t. space ' '
    while(getline(check1, intermediate, ' '))
    {
        tokens.push_back(intermediate);
    }
    if (tokens.empty() == 1){tokens.push_back(s);}
    return tokens; 
}  

void copy_vecs (vector <bool> &base , vector <bool> &dest){
    for (int i = 0; i <base.size(); i++){
        dest[i] = base[i];
    }
}
bool compute_result(vector <string> v){
    string op;
    vector<bool> ins;
    ins.clear(); 

    for (string s: v){
        //cout << "s = " << s << endl;
        if (s == "|" || s == "&"){op = s; continue;}
        for (char c : s){
        if (c == '~'){ins.push_back(1); break; /*cout << "(HERE 1)"<<endl;*/}
        else if (c == s[s.length()-1]){ins.push_back(0); /*cout << "(HERE 0) " << c << " = " << s[s.length()-1] <<endl;*/ break;}
        }
    }
    if (op == "|"){cout << "RESULT of inputs OR gate  "; cout << endl << endl; /*printer_bool(ins);*/ return logic_OR(ins, 0);}
    if (op == "&"){cout << "RESULT of inputs AND gate "; cout << endl << endl; /*printer_bool(ins);*/ return logic_AND(ins, 0);}
    else{return ins[0];}
    
}
void process(vector <string> individual_gates_with_operations){
printer<string>(individual_gates_with_operations);
int flag = 0;
int num_neg = 0;
int num_pos = 0;
char op;

vector <bool> final_vec;
vector <bool> inputs;
string maj_operator;

for (string exp : individual_gates_with_operations){
    vector <string> small_tokens = space_tokenizer(exp);
    if (exp == "|"  || exp == "&"){maj_operator = exp; continue;}
    final_vec.push_back(compute_result(small_tokens));
    cout << "=============================\n";
    bool x = compute_result(small_tokens);
    cout << x;
    cout << "=============================\n";

    }

if (maj_operator == "|"){cout << "FINAL: RES of OR IS " << logic_OR(final_vec, 0) << endl;}
if (maj_operator == "&"){cout << "FINAL: RES of AND IS " << logic_AND(final_vec, 0) << endl;}

}
int main(){
ifstream r("gates.txt");
string x;
getline(r,x);
r.close();
vector<string> individual_gates_with_operations = tokenizer(x);
process(individual_gates_with_operations);
}