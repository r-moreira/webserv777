#ifndef ExecPython_h
#define ExecPython_h

#include "Exec.h"

class ExecPython : public Exec
{
private:
    char* const* _command;
    char* const* _envVariables;
    virtual void script_exec(int stdOut, int stdIn);
public:
    ExecPython(char* const* command, char* const* env);
    ~ExecPython();
};
#endif