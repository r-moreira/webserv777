#include "../../includes/cgi/Exec.h"


Exec::Exec(char * path_bin, char* const* command, char* const* env): _path(path_bin), _cmd(command), _env(env) {}

Exec::~Exec() {
    close(_pipeFd[0]);
}

int Exec::pipe_init() {
    if (pipe(_pipeFd) < 0) {
        Logger::error("Error: unable to Pipe");
        return -1;
    }
    return 0;
}



void Exec::is_infity_loop() {
    clock_t start = clock();

    while (true) {
        double total = (double) (clock() - start) / CLOCKS_PER_SEC;
        if (total > LIMIT_IN_SECONDS) {
            if (waitpid(_pid, &_status, WNOHANG) == 0) {
                _httpStatusCode = 508;
                kill(_pid, SIGSEGV);
                return ;
            }
            kill(_pid, SIGSEGV);
            break;
        }
        if (waitpid(_pid, &_status, WNOHANG) == _pid) {
            break;
        }
    }
}

void Exec::_handleScript(int std_out_from_server) {
    _pid = fork();
    if (_pid == -1) {
        Logger::error("Error: unable to fork");
        return ;
    } else if (!_pid) {
        script_exec(std_out_from_server, _pipeFd[1]);
    } else {
        is_infity_loop();
        if (WEXITSTATUS(_status) != 0) {
            _httpStatusCode = 500;
            kill(_pid, SIGSEGV);
            return ;
        }
        waitpid(_pid, NULL, 0);
    }
    close(_pipeFd[1]);
    _httpStatusCode = 200;
}

int Exec::getStdOut() {
    return _pipeFd[0];
}

int Exec::getStdIn() {
    return _pipeFd[1];
}

int Exec::getHttpStatusCode() {
    return _httpStatusCode; 
}

void Exec::script_exec(int stdIn, int stdOut) {
    dup2(stdIn, STDIN_FILENO);
    dup2(stdOut, STDOUT_FILENO);

    char* errorMessage = (char *)"Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r\n";

    execve(_path, _cmd, _env);
    write(STDOUT_FILENO, errorMessage, 64);
}

void Exec::start(int std_out_from_server) {
    if (pipe_init() < 0)
        return ;
    _handleScript(std_out_from_server);
}