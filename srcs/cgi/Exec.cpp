#include "../../includes/cgi/Exec.h"


Exec::Exec(char * path_bin, char* const* command, char* const* env): _path(path_bin), _cmd(command), _env(env) {}

Exec::~Exec() {
    close(_pipeFd[0]);
}

int Exec::pipe_init() {
    if (pipe(_pipeFd) < 0) {
        std::cout << RED << "Error: unable to Pipe" << RESET << '\n';
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

void Exec::_handleScript() {
    _pid = fork();
    if (_pid == -1) {
        std::cout << RED << "Error: unable to fork" << RESET << '\n';
        return ;
    } else if (!_pid) {
        script_exec(_pipeFd[1]);
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

int Exec::getHttpStatusCode() {
    return _httpStatusCode; 
}

void Exec::script_exec(int stdOut) {
    dup2(stdOut, STDOUT_FILENO);

    char* errorMessage = "Content-Type: text/html\r\n\r Status: 500 Internal Server Error\r\n\r\n";
    char * const cmd[] = {"python3", "tt.py", NULL};

    execve("/usr/bin/python3", cmd, NULL);
    write(STDOUT_FILENO, errorMessage, 64);
}

void Exec::start() {
    if (pipe_init() < 0)
        return ;
    _handleScript();
}