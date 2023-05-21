#ifndef exec_hpp
#define exec_hpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

#include "../webserv.h"
#define LIMIT_IN_SECONDS 5

class Exec
{
private:
    int _pipeFd[2];
    pid_t _pid;
    int _status;
    int _httpStatusCode;
    char * _path;
    char* const* _cmd;
    char* const* _env;

    int pipe_init();
    void _handleScript();
    void is_infity_loop();
    virtual void script_exec(int stdOut);
public:
    Exec(char * path_bin, char* const* command, char* const* env);
    ~Exec();
    void start();
    int getStdOut();
    int getHttpStatusCode();
};
#endif