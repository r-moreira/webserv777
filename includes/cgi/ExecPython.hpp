#ifndef ExecPython_hpp
#define ExecPython_hpp

#include "Exec.hpp"

class ExecPython : Exec
{
private:
    char* const* _command;
    char* const* _envVariables;
    void script_exec(int stdIn, int stdOut);
public:
    ExecPython(char* const* command, char* const* env);
};
#endif