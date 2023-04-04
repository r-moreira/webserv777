#include "../../includes/MultPlexing/MultPlexing.hpp"
#include <cstring>

FT::Multplexing::Multplexing() {
    epoll = epoll_create1(0);
    isFdValid(epoll);
}

FT::Multplexing::~Multplexing(){}

void FT::Multplexing::isFdValid(int fd){
    if (fd == -1) {
        std::cerr << "Error while creating epoll fd: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void FT::Multplexing::register_epoll(int op, int fd, struct epoll_event *event) {
     if (epoll_ctl(epoll, op, fd, event) == -1) {
        std::cerr << "Error while adding client to epoll: " << errno << std::endl;
    }
}

void FT::Multplexing::newRequest(int fd) {
    RequestData *data = new RequestData;
    data->status = Reading;
    data->fd = fd;

    request_event.data.ptr = data;
    request_event.events = EPOLLIN;
    register_epoll(EPOLL_CTL_ADD, fd, &request_event);
    wait();
}

void FT::Multplexing::reading(RequestData *data) {
    data->request = new HttpRequest(data->fd);
    std::cout << data->request->get_body() << '\n';
    std::cout << data->request->get_header() << '\n';
    data->status = Writing;
}

void FT::Multplexing::wiriting(RequestData *data) {
    char *str = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(data->fd , str , strlen(str));
    data->status = Ended;
}

void FT::Multplexing::process_event(RequestData *data) {
    if(data->status == Reading) {
        reading(data);
    }
    if (data->status == Writing) {
        wiriting(data);
    }
}

void FT::Multplexing::wait() {
    for (int i = 0; i < requestEventCount; i++) {
        requestEventCount = epoll_wait(epoll, epoll_list, MAX_EPOLL_EVENTS, -1);
        RequestData *data = (RequestData *) epoll_list[i].data.ptr;
        process_event(data);
        switch (data->status) {
            case Reading:
                request_event.events = EPOLLIN;
                register_epoll(EPOLL_CTL_MOD, data->fd, &request_event);
                break;
            case Writing:
                request_event.events = EPOLLOUT;
                register_epoll(EPOLL_CTL_MOD, data->fd, &request_event);
                break;
            case Ended:
                close(data->fd);
                delete data->request;
                delete data;
        }
    }
}