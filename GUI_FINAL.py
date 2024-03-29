import tkinter as tk
import subprocess
import string
from tkinter import filedialog

file_names = []
def extractor(s):
# Find the last occurrence of '/' to get the index of the file name
    last_slash_index = s.rfind('/')
# Extract the substring starting from the index of the last occurrence of '/'
    file_name = s[last_slash_index + 1:]
    return file_name


def execute_command(command_string):
    try:
        # Execute the command in the shell, capture output
        result = subprocess.run(command_string, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        # Decode the output bytes to string
        output = result.stdout.decode('utf-8')
        error = result.stderr.decode('utf-8')
        
        # Print output and error, if any
        if output:
            print("Output:")
            print(output)
        if error:
            print("Error:")
            print(error)
            
    except subprocess.CalledProcessError as e:
        # Handle errors
        print(f"Error executing command '{command_string}': {e}")

execute_command("g++ -o main_project main_project.cpp")

def go():
    command2 = ".\main_project " + extractor(file_names[0]) + " " + extractor(file_names[1])+ " " + extractor(file_names[2])
    execute_command(command2)
    lst = []


def upload_file():
    filename = filedialog.askopenfilename()
    if filename:
        label.config(text="File uploaded: " + filename)
        file_names.append(filename)
    else:
        label.config(text="No file selected.")

# Create the main window
root = tk.Tk()
root.title("File Uploader")

# Create a label to display the uploaded file path
label = tk.Label(root, text="No file selected.", padx=10, pady=10)
label.pack()

# Create a button to trigger file upload
button = tk.Button(root, text="Upload LIB File", command=upload_file)
button.pack()

button = tk.Button(root, text="Upload CIR File", command=upload_file)
button.pack()

button = tk.Button(root, text="Upload STIM File", command=upload_file)
button.pack()

button = tk.Button(root, text="Run Code", command=go)
button.pack()

button = tk.Button(root, text="Simulation File", command=upload_file)
button.pack()

root.mainloop()
# Run the GUI