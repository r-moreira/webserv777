#include "../../includes/cgi/ExecPython.hpp"

ExecPython::ExecPython(char* const* command, char* const* env): Exec() {
    _command = command;
    _envVariables = env;
}

void ExecPython::script_exec(int stdIn, int stdOut) {
    char* errorMessage = "Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r\n";

    dup2(stdIn, STDIN_FILENO);
    dup2(stdOut, STDOUT_FILENO);

    execve("/usr/bin/python3", _command, _envVariables);
    write(STDOUT_FILENO, errorMessage, 64);
}