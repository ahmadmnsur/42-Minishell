![42 beirut](https://media.licdn.com/dms/image/D4E3DAQGxvimpQQFi_A/image-scale_191_1128/0/1695893480399/42_beirut_cover?e=2147483647&v=beta&t=oNrMR2IXjtfu3XXV-uSUW8wI98g19yUUHZ0XuhtX2NE)


# Minishell

## Overview

The **minishell** project is a simple Unix shell implemented in C as part of the 42 School curriculum. This shell simulates basic shell functionality, such as command execution, environment variable handling, piping, and redirection. It provides a minimalist shell experience, allowing the user to interact with the system using familiar shell commands.

## Features

- **Command Execution**: Executes system commands like `ls`, `cd`, and `echo`.
- **Environment Variables**: Supports environment variable management (`export`, `unset`, and variable expansion).
- **Pipes**: Implements basic pipe functionality to connect commands.
- **Redirection**: Handles input/output redirection using `>` and `<`.
- **Built-in Commands**: Implements common built-in shell commands such as `cd`, `exit`, `echo`, and `export`.
- **Exit Status**: Returns the correct exit status for commands.
- **Signal Handling**: Gracefully handles signals like `Ctrl+C` and `Ctrl+\\`.

## Requirements

- **Unix-based Operating System** (Linux, macOS, etc.)
- **C Compiler** (gcc or clang)
- **Make Utility** for building the project
  
## Dependencies

Before installing, ensure you have the necessary tools:

**Installing make and gcc**

Ubuntu/Debian:

```sudo apt-get update```

```sudo apt-get install build-essential```

macOS (via Xcode Command Line Tools):

```xcode-select --install```

Fedora:

```sudo dnf install gcc make```

Arch Linux:

```sudo pacman -S base-devel```


**Installing the GNU Readline Library**

Ubuntu/Debian:

```sudo apt-get install libreadline-dev```

macOS (via Homebrew):

```brew install readline```

For other systems, refer to your package manager’s documentation.

## Installation

1. Clone the repository:

    ```bash
    https://github.com/mohamad-m-jamil/Minishell.git
    ```

2. Navigate to the project directory:

    ```bash
    cd minishell
    ```

3. Build the project using `make`:

    ```bash
    make
    ```

4. Run the shell:

    ```bash
    ./minishell
    ```

## Usage

Once the shell is running, you can enter commands just like in a regular shell. For example:

- `echo Hello, World!` - Prints "Hello, World!" to the terminal.
- `ls -l` - Lists files in the current directory.
- `cd /path/to/directory` - Changes the current directory.
- `exit` - Exits the shell.

You can also use pipes and redirection:

- `cat file.txt | grep "pattern"` - Uses a pipe to search for "pattern" in the contents of `file.txt`.
- `echo "Hello" > output.txt` - Redirects the output of `echo` to a file.

## Notes

- The project does not implement advanced shell features like job control, but it covers the basics of shell functionality, including command execution, environment variable handling, pipes, and redirection.
- The `cd` command does not support relative paths that rely on `~` (tilde), and certain built-ins like `history` or job control are not implemented in this version.

## Contribution

Feel free to fork the repository and create pull requests for improvements, bug fixes, or new features!
