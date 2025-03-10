# Minishell

Introduction

Minishell is a project from the 42 curriculum that involves building a simple shell program in C. The goal is to understand how a command-line interpreter works, manage processes, handle signals, and implement built-in shell commands similar to Bash.

Features

Command execution (handling external commands like ls, echo, etc.).

Built-in commands: cd, echo, pwd, export, unset, env, exit.

Environment variable management.

Input and output redirections (>, >>, <).

Pipe execution (|).

Handling of $? to return the last command's exit status.

Support for && and || logical operators.

Signal handling (Ctrl+C, Ctrl+D, Ctrl+).

Handling of SHLVL to track shell instances.

Installation & Usage

Clone the repository and compile the program:

make
./minishell

Once inside the shell, you can execute commands as you would in Bash:

$ ls -l
$ echo "Hello, World!"
$ export VAR=value
$ echo $VAR
$ cd ..
$ exit

Project Requirements

Must be written in C and follow the Norminette coding style.

No use of fork() outside of execution.

No use of system().

Handle memory leaks and segmentation faults.

Authors

Mohamad Jamil

Ahmad mansour

License

This project is for educational purposes as part of 42 School and is not intended for commercial use.
