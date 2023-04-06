#ifndef MultPlexing_hpp
#define MultPlexing_hpp
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EPOLL_EVENTS 64

#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"

namespace FT {
enum eventStatus {
    Reading,
    Writing,
    Ended
};

struct RequestData {
    int fd;
    HttpRequest *request;
    HttpResponse *response;
    eventStatus status;
};

class Multplexing
{
public:
    Multplexing();
    ~Multplexing();
    void newRequest(int fd);
private:
    int epoll;  
    size_t requestEventCount;
    struct epoll_event request_event;
    struct epoll_event epoll_list[MAX_EPOLL_EVENTS];
    void isFdValid(int fd);
    void register_epoll(int op, int fd, struct epoll_event *event);
    void wait();
    void reading(RequestData *data);
    void wiriting(RequestData *data);
    void ending(RequestData *data);
    void process_event(RequestData *data);
};
}
#endif