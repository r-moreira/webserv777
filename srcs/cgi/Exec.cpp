#include "../../includes/cgi/Exec.hpp"


Exec::Exec() {
    if (pipe_init() < 0)
        return ;
    _handleScript();
}

Exec::~Exec() {
    close(_pipeFd[0]);
    close(_pipeFd[1]);
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
        script_exec(_pipeFd[0], _pipeFd[1]);
    } else {
        is_infity_loop();
        if (WEXITSTATUS(_status) != 0) {
            _httpStatusCode = 500;
            kill(_pid, SIGSEGV);
            return ;
        }
        waitpid(_pid, NULL, 0);
    }
    _httpStatusCode = 200;
}

int Exec::getStdOut() {
    return _pipeFd[1];
}

int Exec::getHttpStatusCode() {
    return _httpStatusCode;
}