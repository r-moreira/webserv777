#ifndef ExecPython_hpp
#define ExecPython_hpp

#include "Exec.hpp"

class ExecPython : public Exec
{
private:
    char* const* _command;
    char* const* _envVariables;
    virtual void script_exec(int stdOut);
public:
    ExecPython(char* const* command, char* const* env);
};
#endif