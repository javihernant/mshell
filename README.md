# Minishell
## Description
A shell is a tool that is used to interact with computers. Previous to the creation of Windows, the only possible way to talk to a computer was to use a shell. A shell, although not as user friendly, generally provides the same functionality as a windowed system. Indeed, it may be even more convenient, as it may allow you to create scripts that lets you automate tasks and make your life easier overall. Basic functionality of a shell would be things like: navigate a file system, see what files are contained in a specific directory or execute programs. This project is about creating a simple bash clone, one of the most used shells in the history of computing.

This shell has been designed to support || and && operators. In order to do that, the shell contains a simple interpreter, including a lexer and a parser.

The lexer's job is to take a raw set of characters and convert that into a set of tokens. Tokens are chunks of characters that together makes them have meaning. Think of them as something akin words. In this specific situation examples of tokens could be: ||, &&, (, ), etc.

The parser groups this so called tokens into "sentences" that make sense. The parser will essentially produce a tree, containing all commands the user inputs in a line, and have those commands structured in a way that the shell is later able to walk through and execute them in a correct manner and order. To create this tree, the parser uses a technic called recurrent descent.

## Features
Supported features include:
- Working history. Use the arrow keys to select other commands used in the past (WIP)
- It reads the `$PATH` environment variable to know all the directories in which to find binaries to execute.
- Lexer handles sequences of characters surrounded by single quotes (') or double quotes (") so they are not interpreted by the shell. Also, it takes escaping characters into account (ie. \\", \\', \\\\)
- Redirections:
	- < to redirect input from a file.
	- \> to redirect output to a file.
	- \>> to redirect output to a file in append mode.
	- << lets you provide a delimiter. Input will be read until that delimiter is found.
- Pipes, to redirect output of a program into the input of another.
- Handling of environment variables (ie. $var)
- Support for $?, to get the return code returned by the last executed program.
- Signal handling to make ctrl-C, ctrl-D and ctrl-\ behave like in bash (WIP)
- Built-ins (WIP):
	- echo with option -n
	- cd
	- pwd
	- export
	- unset
	- env
	- exit
- Operators && and || are supported. Parentheses are also supported if a change of the default precedence is wanted.
- Wildcards (*) support for current directory.

## Usage
Build the project with `make`.
Then execute the binary (`minishell`) and use it as you would any other shell.

