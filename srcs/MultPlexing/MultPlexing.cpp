#include "../../includes/MultPlexing/MultPlexing.hpp"

FT::Multplexing::Multplexing() {
    epoll = epoll_create1(0);
    isFdValid(epoll);
    requestEventCount = epoll_wait(epoll, epoll_list, MAX_EPOLL_EVENTS, -1);
}

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

void FT::Multplexing::wait() {
    for (int i = 0; i < requestEventCount; i++) {
        RequestData *data = (RequestData *) epoll_list[i].data.ptr;

        switch (data->status) {
            case Reading:
                request_event.events = EPOLLIN;
                reading(data->fd);
                register_epoll(EPOLL_CTL_MOD, data->fd, &request_event);
                break;
            case Writing:
                request_event.events = EPOLLOUT;
                wiriting(data->fd);
                register_epoll(EPOLL_CTL_MOD, data->fd, &request_event);
                break;
            case Ended:
                close(data->fd);
                delete data;
        }
    }
}