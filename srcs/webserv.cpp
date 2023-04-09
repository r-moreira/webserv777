#include "../includes/webserv.h"
#include "../includes/network/Socket.h"
#include "../includes/domain/Event.h"


std::string file_extension[] = {
        "aac", "avi", "bmp", "css", "gif", "ico", "js",
        "json", "mp3", "mp4", "otf", "png", "php", "rtf",
        "svg", "txt", "webm", "webp", "woff", "woff", "zip",
        "html", "htm", "jpeg", "jpg",
};

std::string content_type[] = {
        "Content-Type: audio/aac\r\n\r\n",
        "Content-Type: video/x-msvideo\r\n\r\n",
        "Content-Type: image/bmp\r\n\r\n",
        "Content-Type: text/css\r\n\r\n",
        "Content-Type: image/gif\r\n\r\n",
        "Content-Type: image/vnd.microsoft.icon\r\n\r\n",
        "Content-Type: text/javascript\r\n\r\n",
        "Content-Type: application/json\r\n\r\n",
        "Content-Type: audio/mpeg\r\n\r\n",
        "Content-Type: video/mp4\r\n\r\n",
        "Content-Type: font/otf\r\n\r\n",
        "Content-Type: image/png\r\n\r\n",
        "Content-Type: application/x-httpd-php\r\n\r\n",
        "Content-Type: application/rtf\r\n\r\n",
        "Content-Type: image/svg+xml\r\n\r\n",
        "Content-Type: text/plain\r\n\r\n",
        "Content-Type: video/webm\r\n\r\n",
        "Content-Type: video/webp\r\n\r\n",
        "Content-Type: font/woff\r\n\r\n",
        "Content-Type: font/woff2\r\n\r\n",
        "Content-Type: application/zip\r\n\r\n",
        "Content-Type: text/html\r\n\r\n",
        "Content-Type: text/html\r\n\r\n",
        "Content-Type: image/jpeg\r\n\r\n",
        "Content-Type: image/jpeg\r\n\r\n",
};

std::string getHeaders(const std::string& file_path, size_t file_size) {
    char size_t_byte_buffer[25] = {};
    std::string headers = "HTTP/1.1 200 Ok\r\n";

    if (snprintf(size_t_byte_buffer, 25, "%lu", file_size) < 0) {
        std::cerr << RED << "Error while formatting file size: " << strerror(errno) << RESET << std::endl;
        //stop connection, display error page
    }
    std::string content_length = "Content-Length: " + std::string(size_t_byte_buffer) + "\r\n";

    std::string extension = file_path.substr(file_path.find_last_of('.') + 1);
    for (int i = 0; i < 25; i++) {
        if (extension == file_extension[i]) {
            headers += content_length;
            headers += content_type[i];
            return headers;
        }
    }
    return "HTTP/1.1 200 Ok\r\n" + content_length + "Content-Type: text/html\r\n\r\n";
}

int open_file(Event *event) {
    if (event->getFile() == NULL) {
        event->setFilePath("./public" + event->getRequest().getUri());

        FILE *fptr;
        fptr = fopen(event->getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            std::cerr << RED << "Error while opening file: " << event->getFilePath() << " " << strerror(errno) << RESET << std::endl;
            //return error page, end connection
            event->setEventStatus(Ended);
            return -1;
        }
        event->setFile(fptr);
        //Por algum motivo readbytes precisa ser inicializado somente neste momento
        event->setReadBytes(0);
    }
    return 1;
}

void write_response(Event *event) {
    UrlParser urlParser;

    struct stat file_stat;
    char buffer[30720];

    if (open_file(event) == -1)
        return;

    int fd = fileno(event->getFile());
    if (fd < 1) {
        std::cerr << RED << "Error while getting file descriptor: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
    }

    fstat(fd, &file_stat);

    if (event->getWriteIteration() == 0) {
        std::string headers = getHeaders(event->getFilePath(), file_stat.st_size);
        std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

        if (send(event->getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
            std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET
                      << std::endl;
            //return error page, end connection
        }

        std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
    }

    size_t read_size;
    if (file_stat.st_size > 30720) {
        read_size = event->getReadLeft() > 30720 ? 30720 : event->getReadLeft();
    } else {
        read_size = file_stat.st_size;
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t read_bytes = fread(buffer, 1, read_size, event->getFile());

    event->setReadBytes(event->getReadBytes() + read_bytes);
    std::cout << YELLOW << "Readed Data Size: " << read_bytes << RESET << std::endl;

    event->setReadLeft(file_stat.st_size - event->getReadBytes());
    std::cout << YELLOW << "Read Left: " << event->getReadLeft() << RESET << std::endl;

    long bytes_sent = send(event->getClientFd() ,buffer, read_bytes, 0 );
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
        event->setEventStatus(Ended);
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;

    event->setWriteIteration(event->getWriteIteration() + 1);
    if (event->getReadLeft() <= 0) {
        event->setEventStatus(Ended);
    }
}

Request parse_request(Event *event) {

    std::cout << MAGENTA << "Request Data:\n " << event->getReadBuffer() << RESET << std::endl;

    const char *buffer = event->getReadBuffer().c_str();

    Request request;
    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << request.inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        //Return error page, end connection
    }
    event->setEventStatus(Writing);
    return request;
}

void read_request(Event *event) {
    char buffer[READ_BUFFER_SIZE] = {};
    Request request;

    long bytes_read = read(event->getClientFd(), buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        event->setEventStatus(Ended);
        return;
    }

    event->setReadBytes(event->getReadBytes() + bytes_read);
    std::string read_buffer = event->getReadBuffer();
    event->setReadBuffer(read_buffer.append(buffer));

    std::cout << YELLOW << "Read " << event->getReadBytes() << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << buffer << RESET << std::endl;

    if (buffer[READ_BUFFER_SIZE - 1] == 0) {
        request = parse_request(event);
    }
    event->setRequest(request);
}

void process_event(Event *event) {
    if (event->getEventStatus() == Reading) {
        read_request(event);
    } else if (event->getEventStatus() == Writing) {
        write_response(event);
    }
}

void io_multiplexing_event_loop(int server_socket_fd) {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << RED << "Error while creating epoll fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    struct epoll_event server_event = {};
    struct epoll_event request_event = {};

    server_event.events = EPOLLIN;
    server_event.data.fd = server_socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_event) == -1) {
        std::cerr << RED << "Error while adding socket to epoll: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    struct epoll_event epoll_events[MAX_EPOLL_EVENTS];
    while (1) {
        int nfds = epoll_wait(epoll_fd, epoll_events, MAX_EPOLL_EVENTS, 3000);
        if (nfds == -1) {
            std::cerr << RED << "Error while waiting for epoll events: " << strerror(errno) << RESET << std::endl;
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (epoll_events[i].data.fd == server_socket_fd) {

                int client_fd = Socket::setupClient(server_socket_fd);
                if (client_fd < 0) break;

                Event *event = new Event(client_fd);
                request_event.data.ptr = event;
                request_event.events = EPOLLIN;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &request_event) == -1) {
                    std::cerr << RED << "Error while adding client to epoll: " << strerror(errno) << RESET << std::endl;
                    continue;
                }
            } else {
                Event *event = (Event *) epoll_events[i].data.ptr;
                process_event(event);
                switch (event->getEventStatus()) {
                    case Reading:
                        request_event.events = EPOLLIN;
                        request_event.data.ptr = event;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &request_event) < 0) {
                            std::cerr << RED << "Error while adding reading event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Writing:
                        request_event.events = EPOLLOUT;
                        request_event.data.ptr = event;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event->getClientFd(), &request_event) < 0) {
                            std::cerr << RED << "Error while adding writing event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Ended:
                        if (event->getFile() != NULL) {
                            fclose(event->getFile());
                            event->setFile(NULL);
                        }
                        close(event->getClientFd());
                        delete event;
                }

            }
        }
    }
}

int main(int argc, char **argv, char **env) {
    signal(SIGPIPE, SIG_IGN);

    if (argc != 2) {
        std::cout << CYAN << "Usage :  ./webserv {PATH TO CONFIGURATION FILE}" << RESET << std::endl;
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    io_multiplexing_event_loop(Socket::setupServer(8080 + (rand() % 10)));

    return EXIT_SUCCESS;
}