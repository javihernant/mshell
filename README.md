# Minishell
## Description
Create a shell (similar to bash), that can execute programs.

This shell accepts command concatenation with operators like || or &&. A simple interpreter has been implemented to make it work.

## Build
Build the project with `make`.
Then execute the executable that it creates (minishell)

## TODO
- FIX: tab recognized as an empty arg
test_exec_cmd("ls 	*.o 	\'ojfore $?\' \'$?\' \"joifjre$?foijo\" \"$?\" ijforij$? $?jojoi $?");
'ls' '' 'ojfore $?' '$?' 'joifjre$?foijo' '$?' 'ijforij$?' '$?jojoi' '$?'

