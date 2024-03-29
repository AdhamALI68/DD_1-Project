import subprocess

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

# First command
command1 = "g++ -o main_project main_project.cpp"
execute_command(command1)

# Second command
command2 = "main_project cells.lib 2.cir 2.stim"
execute_command(command2)
