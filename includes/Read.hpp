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
    int getSize();
    std::string getText();
private:
    char buffer[BUFFER_SIZE];
    int size;
    int is_read;
    std::string text;
    void readig_fd(int fd);
};

}
#endif