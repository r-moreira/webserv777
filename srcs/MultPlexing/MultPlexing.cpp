#include "../../includes/MultPlexing/MultPlexing.hpp"
#include <cstring>

FT::Multplexing::Multplexing(): ServerTemplate(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 100){
    epoll = epoll_create1(0);
    isFdValid(epoll);
    serverSocketFd = get_socket()->get_socket();
    server_event.events = EPOLLIN | EPOLLET;
    server_event.data.fd = serverSocketFd;
    register_epoll(EPOLL_CTL_ADD, serverSocketFd, &server_event);
    launch();
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

void FT::Multplexing::reading(RequestData *data) {
    data->request = new HttpRequest(data->fd);
    std::cout << data->request->get_body() << '\n';
    std::cout << data->request->get_header() << '\n';
    data->status = Writing;
}

void FT::Multplexing::wiriting(RequestData *data) {
    data->response = new HttpResponse(data->request);
    write(data->fd , data->response->get_response().data() , data->response->get_response().size());
    data->status = Ended;
}

void FT::Multplexing::accepter() {
    int fd = get_socket()->socket_accepter();
    if (fd < 0)
        return ;
    RequestData *data = new RequestData;

    data->status = Reading;
    data->fd = fd;
    request_event.data.ptr = data;
    request_event.events = EPOLLIN;
    register_epoll(EPOLL_CTL_ADD, data->fd, &request_event);
}

void FT::Multplexing::process_event(RequestData *data) {
    if(data->status == Reading) {
        reading(data);
    }
    else if (data->status == Writing) {
        wiriting(data);
    }
}

void FT::Multplexing::launch() {
    while (1)
    {
        std::cout << "================ LISTENING ================" << std::endl;
        wait();
        std::cout << "================ DONE ================" << std::endl;
    }
}

void FT::Multplexing::wait() {
    requestEventCount = epoll_wait(epoll, epoll_list, MAX_EPOLL_EVENTS, -1);
    for (int i = 0; i < requestEventCount; i++) {
        if (epoll_list[i].data.fd == serverSocketFd) {
            int fd = get_socket()->socket_accepter();
            if (fd < 0)
                break ;
            RequestData *data = new RequestData;

            data->status = Reading;
            data->fd = fd;
            request_event.data.ptr = data;
            request_event.events = EPOLLIN;
            register_epoll(EPOLL_CTL_ADD, data->fd, &request_event);
        }
        else {
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
                    delete data->response;
                    delete data;
            }
        }
    }
}