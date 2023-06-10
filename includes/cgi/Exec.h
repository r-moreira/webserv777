#ifndef exec_h
#define exec_h

#include "../utils/Logger.h"
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
    void _handleScript(int std_out_from_server);
    int is_infity_loop();
    virtual void script_exec(int stdOut, int stdIn);
public:
    Exec(char * path_bin, char* const* command, char* const* env);
    virtual ~Exec();
    void start(int std_out_from_server);
    int getStdOut();
    int getStdIn();
    int getHttpStatusCode();
};
#endif