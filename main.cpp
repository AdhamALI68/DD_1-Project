#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <unordered_map>
#include <tuple>

using namespace std;

// Define a type for logic functions
using LogicFunction = function<bool(const vector<bool>&, int)>;

// Basic logic functions with delay parameter
bool logic_AND(const vector<bool>& inputs, int delay) {
    bool result = true;
    for (bool input : inputs) {
        result = result && input;
    }
    return result;
}

bool logic_OR(const vector<bool>& inputs, int delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result || input;
    }
    return result;
}

bool logic_NAND(const vector<bool>& inputs, int delay) {
    bool result = !(logic_AND(inputs, delay));
    return result;
}

bool logic_XOR(const vector<bool>& inputs, int delay) {
    bool result = false;
    for (bool input : inputs) {
        result = result ^ input;
    }
    return result;
}

bool logic_NOT(const vector<bool>& inputs, int delay) {
    vector<bool> inverted_inputs = inputs;
    for (int i = 0; i < inputs.size(); ++i) {
        inverted_inputs[i] = !inverted_inputs[i];
    }
    return inverted_inputs[0];
}

bool logic_NOR(const vector<bool>& inputs, int delay) {
    bool result = !(logic_OR(inputs, delay));
    return result;
}

// Map to store logic functions
unordered_map<string, LogicFunction> logic_functions = {
    {"AND", logic_AND},
    {"OR", logic_OR},
    {"NAND", logic_NAND},
    {"XOR", logic_XOR},
    {"NOT", logic_NOT},
    {"NOR", logic_NOR}
};

// Structure to hold component information
struct Component {
    int num_inputs;
    int delay_ps;
};

// Map to store components
unordered_map<string, Component> component_library;

// Load library file and store propagation delay and number of inputs
void loadLibrary(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open library file: " << filename << endl;
        exit(1);
    }
    string line;
    // Read each line from the library file
    while (getline(file, line)) {
        stringstream ss(line);
        string firstWord, lastWord, word;
        // Extract the first and last words from the line
        ss >> firstWord;
        while (ss >> word) {
            lastWord = word;
        }
        // Convert the last word to an integer
        int lastInteger = stoi(lastWord);
        // Store component information in the library map
        component_library[firstWord] = {firstWord[0], lastInteger};
    }
    file.close();
}

// Read input file and extract data
vector<tuple<int, char, int>> readFromFile(const string& filename) {
    vector<tuple<int, char, int>> readings;
    ifstream file(filename);
    string line;
    // Read each line from the input file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        int value1;
        char value2;
        int value3;
        // Parse the comma-separated values from the line
        if (getline(ss, token, ','))
            value1 = stoi(token);
        if (getline(ss, token, ','))
            value2 = token[1];
        if (getline(ss, token, ','))
            value3 = stoi(token);
        // Store the parsed values in a tuple and add it to the readings vector
        readings.push_back(make_tuple(value1, value2, value3));
    }
    file.close();
    return readings;
}

// Parse circuit file and extract inputs, components, and connections
void parse_cir_file(const string& filename, vector<string>& inputs, vector<vector<string>>& components, vector<vector<string>>& ins) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open input file: " << filename << endl;
        exit(1);
    }
    string line;
    bool reading_inputs = false;
    bool reading_components = false;
    // Read each line from the circuit file
    while (getline(file, line)) {
        // Check if currently reading inputs section
        if (line == "INPUTS:") {
            reading_inputs = true;
            continue;
        }
        // Check if currently reading components section
        if (line == "COMPONENTS:") {
            reading_inputs = false;
            reading_components = true;
            continue;
        }
        // Store input names if currently reading inputs section
        if (reading_inputs && !line.empty()) {
            inputs.push_back(line);
        }
        // Parse components and their connections if currently reading components section
        if (reading_components && !line.empty()) {
            stringstream ss(line);
            vector<string> component;
            string token;
            vector<string> vv;
            // Parse each token (component name or connection) from the line
            while (ss >> token) {
                component.push_back(token);
                // Store connections if the component section is reached
                if (component.size() >= 4) {
                    vv.push_back(token);
                }
            }
            // Store connections for each component
            ins.push_back(vv);
            // Store the parsed component
            components.push_back(component);
        }
        // Clean up connections by removing trailing commas or spaces
        for (int i = 0; i < ins.size(); i++) {
            for (int j = 0; j < ins[i].size(); j++) {
                if (ins[i][j][ins[i][j].length() - 1] == ',') {
                    ins[i][j].pop_back();
                }
                if (ins[i][j][ins[i][j].length() - 1] == ' ') {
                    ins[i][j].pop_back();
                }
                if (ins[i][j][ins[i][j].length() - 1] == ',') {
                    ins[i][j].pop_back();
                }
            }
        }
    }
    file.close();
}

// Execute the circuit components based on input values
void execute_circuit(vector<tuple<string,string,vector<bool>>> vec, unordered_map<string, int> input_map, vector<vector<string>> ins) {
    for (int i = 0; i < vec.size(); i++) {
        string gatename = std::get<0>(vec[i]);
        string output = std::get<1>(vec[i]);
        vector<bool> inp;
        // Prepare inputs for the current component
        for (int j = 0;  j < ins[i].size(); j++) {
            inp.push_back(input_map[ins[i][j]]);
        }
        string z = gatename + ',';
        // Execute the appropriate logic function based on the gate name
        if (gatename == "NOT") {
            input_map[output] = logic_NOT(inp, component_library[z].delay_ps);
        } else if (gatename == "AND") {
            input_map[output] = logic_AND(inp, component_library[z].delay_ps);
        } else if (gatename == "OR") {
            input_map[output] = logic_OR(inp, component_library[z].delay_ps);
        } else if (gatename == "XOR") {
            input_map[output] = logic_XOR(inp, component_library[z].delay_ps);
        } else if (gatename == "NAND") {
            input_map[output] = logic_NAND(inp, component_library[z].delay_ps);
        }else if (gatename == "NOR") {
            input_map[output] = logic_NOR(inp, component_library[z].delay_ps);
        }
    }
}

int main() {
    // Load library
    loadLibrary("Circuit 3/Circuit 3.lib.txt");

    // Parse circuit file
    vector<string> inputs;
    vector<vector<string>> components;
    vector<vector<string>> ins;
    parse_cir_file("Circuit 3/Circuit 3.cir.txt", inputs, components, ins);

    // Prepare input map
    unordered_map<string, int> input_map;
    for (const string& input : inputs) {
        input_map[input] = 0;
    }

    // Extract component functions and number of parameters
    unordered_map<string, int> component_functions;
    for (const auto& component : components) {
        string function_name = component[1];
        // Store the number of parameters for each component function
        component_functions[function_name] = component.size() - 3;
    }

    // Prepare vectors to store component details
    vector<tuple<string,string,vector<bool>>> vec;
    unordered_map<string, int> delay_map;

    // Iterate through components, execute logic, and update input map
    for (int i = 0; i < components.size(); i++) {
        string output = "";
        string gatename = "";
        vector<string> in;
        int max_delay = 0;
        for (int j = 0; j < components[i].size(); j++) {
            if (component_functions[components[i][j]] != 0) {
                if (gatename == "") {
                    // Store the gate name
                    gatename = components[i][j];
                    gatename.pop_back(); // Remove the colon
                }
                // Store the output wire name
                output = components[i][j + 1];
                j++; // Skip the output wire name
                continue;
            }
            if (output != "") {
                // Store the input wire names
                if (components[i][j][components[i][j].length() - 1] == ',') {
                    components[i][j].erase(components[i][j].length() - 1);
                }
                in.push_back(components[i][j]);
                // Initialize input wires to zero if not present in the map
                if (input_map.find(components[i][j]) == input_map.end()) {
                    input_map[components[i][j]] = 0;
                }
            }
        }
        // Prepare inputs for the current component
        vector<bool> inp;
        for (int k = 0; k < in.size(); k++) {
            inp.push_back(input_map[in[k]]);
            // Update the maximum delay for this component based on the input wire delays
            if (delay_map.find(in[k]) != delay_map.end()) {
                max_delay = max(max_delay, delay_map[in[k]]);
            }
        }
        // Get the logic function delay for the current gate
        string z = gatename + ',';
        output.pop_back(); // Remove the colon from the output wire name
        // Store the component details for later execution
        vec.push_back({gatename, output, inp});
        // Execute the logic function and update the delay for the output wire
        if (gatename == "NOT") {
            input_map[output] = logic_NOT(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        } else if (gatename == "AND") {
            input_map[output] = logic_AND(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        } else if (gatename == "OR") {
            input_map[output] = logic_OR(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        } else if (gatename == "XOR") {
            input_map[output] = logic_XOR(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        } else if (gatename == "NAND") {
            input_map[output] = logic_NAND(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        }else if (gatename == "N0R") {
            input_map[output] = logic_NOR(inp, component_library[z].delay_ps);
            delay_map[output] = max_delay + component_library[z].delay_ps;
        // Clear the input wires for the next component
        in.clear();
    }

    // Read stimulus file
    vector<tuple<int, char, int>> readings = readFromFile("Circuit 3/Circuit 3.stim");

    // Simulate circuit for each set of inputs
    for (const auto& reading : readings) {
        string c = "";
        c += get<1>(reading);
        // Update input map with stimulus values
        input_map[c] = get<2>(reading);
        // Execute the circuit logic with the updated inputs
        execute_circuit(vec, input_map, ins);

        // Output the results
        cout << "Time: " << get<0>(reading) << " " << get<1>(reading) << " = " << get<2>(reading) << endl;

        // Display the updated state of each wire
        for (const auto& input : inputs) {
            cout << input << " = " << input_map[input] << endl;
        }
        // Output the final state of the gates
        for (const auto& gate : vec) {
            cout << get<1>(gate) << " = " << input_map[get<1>(gate)] << endl;
        }
        // Calculate and output the maximum propagation delay
        int max_delay = 0;
        for (const auto& delay : delay_map) {
        max_delay = max(max_delay, delay.second);
        }
        cout << "Maximum Propagation Delay: " << max_delay << " ps" << endl;

        

        // Output a blank line for better readability
        cout << endl;
    }

    return 0;
}


        
