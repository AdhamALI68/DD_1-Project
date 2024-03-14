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
        stringstream ss(line);
        string name,b,c;
        int num_inputs, delay_ps;
        ss >> name >> num_inputs >> b>>c>>delay_ps;
        component_library[name] = {num_inputs, delay_ps};
    }
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
    while (getline(file, line)) {
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

double simulate_circuit(const vector<vector<string>>& components, unordered_map<string, int>& input_map) {
    int total_delay = 0;

    // Iterate through components
    for (const auto& component : components) {
        string gate_name = component[0];
        string function_name = component[1];
        string output_wire = component[2];

        // Get delay and logic function for the component
        int delay = component_library[function_name].delay_ps;
        LogicFunction logic_function = logic_functions[function_name];

        // Extract input wires
        vector<string> input_wires;
        for (size_t i = 3; i < component.size(); ++i) {
            if (component[i] != ",") {
                input_wires.push_back(component[i]);
            }
        }

        // Get input values from input_map
        vector<bool> inputs;
        for (const auto& input_wire : input_wires) {
            inputs.push_back(input_map[input_wire]);
        }

        // Perform logic operation and update output wire value in input_map
        bool result = logic_function(inputs, delay);
        input_map[output_wire] = result;

        // Accumulate delay
        total_delay += delay;
    }

    return total_delay;
}


int main() {
    loadLibrary("lib.txt");

    // Example usage
    for (const auto& x : component_library) {
        cout << "Component: " << x.first <<" "<<x.second.num_inputs<<" "<<x.second.delay_ps<<"\n"  ;
    }
    vector<string> inputs;
    vector<vector<string>> components;
    parse_cir_file("cir.txt", inputs, components);
    

    // Store inputs in a map and initialize to zero
    unordered_map<string, int> input_map;
    for (const string& input : inputs) {
        input_map[input] = 1;
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
    
  

    for (int i = 0; i < components.size(); i++)
    {
        string output="";
        string gatename="";
        vector<string>in;
        vector<bool>inp;
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
                    }
                    string z=gatename+',';
                    output.pop_back();
                    if(gatename=="NOT"){ input_map[output]=logic_NOT(inp,component_library[z].delay_ps);}
                    gatename.pop_back();
                    if(gatename=="AND"){ input_map[output]=logic_AND(inp,component_library[z].delay_ps);}
                    if(gatename=="OR"){ input_map[output]=logic_OR(inp,component_library[z].delay_ps);}
                    if(gatename=="XOR"){ input_map[output]=logic_XOR(inp,component_library[z].delay_ps);}

                    in.clear();
        cout<<"\n";
    }
    
        cout << "Inputs:" << endl;

    // Printing the component functions and number of parameters
      for (const auto& x: input_map) {
        cout << x.first << ": " << x.second << endl;
    }
    // for (const auto&x : component_functions) {
    //     cout << x.first << ": " << x.second << " parameters" << endl;
    // }





    return 0;
}