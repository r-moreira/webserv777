#ifndef Read_hpp
#define Read_hpp
#include <unistd.h>
#include <iostream>

#define BUFFER_SIZE 30

namespace FT {
class Read
{
public:
    Read(int fd);
    std::string get_text();
private:
    char buffer[BUFFER_SIZE];
    int is_read;
    std::string text;
    void readig_fd(int fd);
};

}
#endif