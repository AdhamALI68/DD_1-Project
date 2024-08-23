# Circuit Simulator

This project is a basic circuit simulation tool written in C++. It reads the definitions of logic gates, circuits, and stimuli from input files, simulates the circuit's behavior based on the stimuli, and outputs the results to a file.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Files](#files)
- [Usage](#usage)
- [Functions Overview](#functions-overview)
- [Error Handling](#error-handling)
- [Compilation and Execution](#compilation-and-execution)

## Introduction

The program simulates the behavior of a digital circuit. It reads gate definitions from a `.lib` file, circuit definitions from a `.cir` file, and stimuli from a `.stim` file. The program processes these files, simulates the circuit’s behavior, and writes the output to a `.sim` file for further analysis or graphing.

## Features

- **File Parsing**: Reads circuit and library files to determine the structure and logic of the circuit.
- **Simulation**: Simulates the circuit's behavior over time based on the given stimuli.
- **Error Handling**: Checks for inconsistencies between the circuit and library files.
- **Output Generation**: Produces a `.sim` file that contains the results of the simulation, suitable for graphing.

## Files

- **`Custom_gates.cpp`**: Contains custom logic gate implementations.
- **`<library_file>.lib`**: Defines the logic and delays of various gates.
- **`<circuit_file>.cir`**: Describes the circuit structure and connections.
- **`<stimulus_file>.stim`**: Provides the input signals over time for the circuit simulation.

## Usage

1. **Modify Library File**: The program first modifies the library file to ensure proper formatting.
2. **Load Library**: The library file is loaded to extract gate logic and delays.
3. **Parse Circuit File**: The circuit file is parsed to understand the components and connections.
4. **Simulation**: The circuit is simulated based on the stimuli provided, and the results are output to a `.sim` file.

### Command Line Usage

```bash
./circuit_simulator <library_file> <circuit_file> <stimulus_file>
