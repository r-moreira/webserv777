#ifndef exec_hpp
#define exec_hpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "../webserv.h"
#define LIMIT_IN_SECONDS 5

class Exec
{
protected:
    virtual void script_exec(int stdIn, int stdOut) = 0;

private:
    int _pipeFd[2];
    pid_t _pid;
    int _status;
    int _httpStatusCode;

    int pipe_init();
    void _handleScript();
    void is_infity_loop();
public:
    Exec();
    ~Exec();
    int getStdOut();
    int getHttpStatusCode();
};
#endif