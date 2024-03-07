#include<bits/stdc++.h>
using namespace std;
using LogicFunction = function<bool(const vector<bool>&, int&)>;

// Define basic logic functions with delay parameter
bool logic_AND(const vector<bool>& inputs, int& delay) {
    bool result = true;
    for (bool input : inputs) {
        result = result && input;
    }
    // Modify delay if needed
    return result;
}

bool logic_OR(const vector<bool>& inputs, int& delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result || input;
    }
    // Modify delay if needed
    return result;
}

bool logic_NAND(const vector<bool>& inputs, int& delay) {
    bool result = !(logic_AND(inputs, delay));
    // Modify delay if needed
    return result;
}

bool logic_XOR(const vector<bool>& inputs, int& delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result ^ input;
    }
    // Modify delay if needed
    return result;
}

bool logic_NOT(const vector<bool>& inputs, int& delay) {
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
    cout << "Inputs:" << endl;
    for (const auto& x: input_map) {
        cout << x.first << ": " << x.second << endl;
    }

    // Printing the component functions and number of parameters
    cout << "Component Functions:" << endl;
    for (const auto&x : component_functions) {
        cout << x.first << ": " << x.second << " parameters" << endl;
    }

    return 0;
}
