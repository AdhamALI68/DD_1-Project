#include <bits/stdc++.h>

using namespace std;

using LogicFunction = function<bool(const vector<bool>&, int)>;

// Define basic logic functions with delay parameter
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
    return result;
}

bool logic_XOR(const vector<bool>& inputs, int delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result ^ input;
    }
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

// Create a map to store logic functions
unordered_map<string, LogicFunction> logic_functions = {
        {"AND", logic_AND},
        {"OR", logic_OR},
        {"NAND", logic_NAND},
        {"XOR", logic_XOR},
        {"NOT", logic_NOT}
};

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
    std::stringstream ss(line);
    std::string firstWord, lastWord, word;

    // Extract the first word
    ss >> firstWord;

    // Extract the last word
    while (ss >> word) {
        lastWord = word; // Overwrite lastWord until the last word is reached
    }
    int lastInteger = std::stoi(lastWord);
cout<<firstWord<<" "<<lastWord;
        component_library[firstWord] = {firstWord[0], lastInteger};
    }

//     for (const auto& x : component_library) {
//    cout << "Component Name: " << x.first << endl;
//    cout << "Number of Inputs: " << x.second.first << endl;
//    cout << "Delay (ps): " << x.second.second << endl;
//}
    file.close();
}


void parse_cir_file(const string& filename, vector<string>& inputs, vector<vector<string>>& components) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open input file: " << filename << endl;
        exit(1);
    }
    string line;
    bool reading_inputs = false;
    bool reading_components = false;
    // line.pop_back();
    while (getline(file, line))
    {
        // line.pop_back();
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
        if (reading_components && !line.empty()) {
            stringstream ss(line);
            vector<string> component;
            string token;
            while (ss >> token) {
                component.push_back(token);
            }
            components.push_back(component);
        }
    }
    file.close();
}

void funccall(vector<tuple<string,string,vector<bool>>>vec, unordered_map<string, int>input_map){
    
    for (int i = 0; i < vec.size(); i++)
    {
vector<bool> inp = std::get<2>(vec[i]);
string gatename = std::get<0>(vec[i]);
string output=std::get<1>(vec[i]);


        string z=gatename+',';
             if(gatename=="NOT"){
            input_map[output]=logic_NOT(inp,component_library[z].delay_ps);
        }
        gatename.pop_back();
        if(gatename=="AND"){
            input_map[output]=logic_AND(inp,component_library[z].delay_ps);
        }
        if(gatename=="OR"){
            input_map[output]=logic_OR(inp,component_library[z].delay_ps);
        }
        if(gatename=="XOR"){
            input_map[output]=logic_XOR(inp,component_library[z].delay_ps);
        }

         if(gatename=="NAND"){
            input_map[output]=logic_XOR(inp,component_library[z].delay_ps);
        }

    }
}
int main() {
    loadLibrary("Circuit 3/Circuit 3.lib.txt");
vector<tuple<string,string,vector<bool>>>vec;

    // Example usage
    for (const auto& x : component_library) {
        cout << "Component: " << x.first <<" "<<x.second.num_inputs<<" "<<x.second.delay_ps<<"\n"  ;
    }
    vector<string> inputs;
    vector<vector<string>> components;
    parse_cir_file("Circuit 3/Circuit 3.cir.txt", inputs, components);


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

    // Example usage:
    // Printing the input map
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
                output=components[i][j+1];          j++; continue; }

            if(output!=""){
                if(components[i][j][components[i][j].length()-1]==','){components[i][j].erase(components[i][j].length()-1);}
                in.push_back(components[i][j]);
                if(input_map.find(components[i][j])==input_map.end()){
                    input_map[components[i][j]]=0;}

            }
        }
        for (int k = 0; k < in.size(); k++)
        {
            inp.push_back(input_map[in[k]]);
            // Update the maximum delay for this component based on the input wire delays
            if (delay_map.find(in[k]) != delay_map.end()) {
                max_delay = max(max_delay, delay_map[in[k]]);
            }
        }
        string z=gatename+',';
        output.pop_back();
        cout<<"out "<<output<<"\n";
        vec.push_back({gatename,output,inp});
        if(gatename=="NOT"){
            input_map[output]=logic_NOT(inp,component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }
        gatename.pop_back();
        if(gatename=="AND"){
            input_map[output]=logic_AND(inp,component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }
        if(gatename=="OR"){
            input_map[output]=logic_OR(inp,component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }
        if(gatename=="XOR"){
            cout<<"out   " <<component_library[z].delay_ps<<"\n";
            input_map[output]=logic_XOR(inp,component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }

         if(gatename=="NAND"){
            cout<<"out   " <<component_library[z].delay_ps<<"\n";
            input_map[output]=logic_XOR(inp,component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }

        in.clear();
        cout<<"\n";
    }
    cout << "Inputs:" << endl;

    // Printing the component functions and number of parameters
    for (const auto& x: input_map) {
        cout << x.first << ": " << x.second << endl;
    }

    cout << "Propagation Delays:" << endl;

    // Printing the propagation delays for each wire
    for (const auto& x: delay_map) {
        cout << x.first << ": " << x.second << " ps" << endl;
    }

    vector<tuple<int, char, int>> readings = readFromFile("Circuit 3/Circuit 3.stim");

    // Printing out the vector to verify the results
    for (const auto& reading : readings) {
        cout << get<0>(reading) << ", " << get<1>(reading) << ", " << get<2>(reading) << endl;
        string c="";
        c+=get<1>(reading);
        input_map[c]=get<2>(reading);
        funccall(vec,input_map);
      cout << "Outputs:" << endl;
      cout<<"iteration beign "<<"\n \n ";

    // Printing the component functions and number of parameters
    for (const auto& x: input_map) {
        cout << x.first << ": " << x.second << endl;
    }
    cout<<"iteration done "<<"\n \n \n";
    }


    return 0;
}