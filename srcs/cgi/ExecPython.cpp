#include "../../includes/cgi/ExecPython.h"

ExecPython::ExecPython(char* const* command, char* const* env): Exec(NULL, NULL, NULL) {
    _command = command;
    _envVariables = env;
}

ExecPython::~ExecPython() {}


void ExecPython::script_exec(int stdOut) {
    dup2(stdOut, STDOUT_FILENO);

    char * errorMessage = (char *)"Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r\n";

    execve("/usr/bin/python3", _command, _envVariables);
    write(STDOUT_FILENO, errorMessage, 64);
}