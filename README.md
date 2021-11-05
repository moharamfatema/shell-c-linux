**A shell on linux that supports the following: **

1. The internal shell command "exit" which terminates the shell 
    Concepts: shell commands, exiting the shell.
    System calls: exit()

2. A command with no arguments 
    Example: `ls` …etc 
    Details: The shell is blocked until the command completes and, if the return code is abnormal, a message to that effect is printed.
    Concepts: Forking a child process, waiting for it to complete and synchronous execution. 
    System calls: `fork()` , `execvp()` , `exit()` , `wait()`

3. A command with arguments 
    Example: `ls –l`, `cp source destination`, `rm filename`. 
    Details: Argument 0 is the name of the command. 
    Concepts: Command-line parameters

4. A command, with or without arguments, executed in the background using &. 
    Example: `firefox &` 
    Details: In this case, the shell executes the command and returns immediately, not blocking until the command finishes. 
    Concepts: Background execution, signals, signal handlers, processes and asynchronous execution.
