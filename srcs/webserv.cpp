#include "../includes/webserv.h"


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

std::string messages[] = {
        "HTTP/1.1 200 Ok\r\n ",
        "HTTP/1.0 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>System is busy right now.</body></html>",
        "HTTP/1.0 404 File not found\r\nContent-Type: text/html\r\n\r\n<!doctype html><html><body>The reuested file does not exist on this server</body></html>"
};


/*
 * * AF_INET is the IP address family of the server
 * * SOCK_STREAM is the type of TCP socket
 * * IPPROTO_TCP is the protocol of the socket
 * * INADDR_ANY accepts any IP address
 * * SOMAXCONN is the maximum number of connections that can be queued
 */
int create_server_socket(int port) {
    struct sockaddr_in sevr_addr = {};
    sevr_addr.sin_family = AF_INET;
    sevr_addr.sin_port = htons(port);
    sevr_addr.sin_addr.s_addr = INADDR_ANY;

    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_fd < 0) {
        std::cerr << RED << "Error while opening server_fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *) &sevr_addr, sizeof(sevr_addr)) != 0) {
        std::cerr << RED << "Error while binding server_fd to address: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, SOMAXCONN) < 0) {
        std::cerr << RED << "Error while listening on server_fd: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << GREEN << "Server started to listen at port: " << port << RESET << std::endl;
    return server_fd;
}

void set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << RED << "Error while getting flags: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        std::cerr << RED << "Error while setting flags: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }
}

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

//TODO:
// * ALTERAR A FORMA DE LER O ARQUIVO
// * A API DE STREAMS() DO C++ NÃO É PERFORMÁTICA
// * USAR FUNÇÕES COMO READ() E WRITE() É MAIS EFICIENTE
// * ESTUDAR SOBRE SEND() X SENDFILE() X WRITE()
void write_response(event_data_t *event_data) {
    UrlParser urlParser;
    std::string file_path = "./public" + event_data->request.getUri();


    std::ifstream file;
    file.open(file_path.c_str(), std::ios::in | std::ios::binary);

    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::cout << YELLOW << " Transmission Data Size " << contents.length() << " Bytes." << RESET << std::endl;

    std::string headers = getHeaders(file_path, contents.length());

    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(event_data->client_fd, headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
    }

    long bytes_sent = send(event_data->client_fd , contents.c_str() , contents.length() , 0 );
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
        event_data->event_status = Ended;
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;

    event_data->event_status = Ended;
}

Request parse_request(event_data_t *event_data) {

    std::cout << MAGENTA << "Request Data:\n " << event_data->read_buffer << RESET << std::endl;

    const char *buffer = event_data->read_buffer.c_str();

    Request request;
    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << request.inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        //Return error page, end connection
    }
    event_data->event_status = Writing;
    return request;
}

void read_request(event_data_t *event_data) {
    int client_fd = event_data->client_fd;
    char buffer[READ_BUFFER_SIZE] = {};
    Request request;

    long bytes_read = read(client_fd, buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        event_data->event_status = Ended;
        return;
    }

    event_data->read_bytes += bytes_read;
    event_data->read_buffer.append(buffer);

    std::cout << YELLOW << "Read " << event_data->read_bytes << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << buffer << RESET << std::endl;

    if (buffer[READ_BUFFER_SIZE - 1] == 0) {
        request = parse_request(event_data);
    }
    event_data->request = request;
}

void process_event(event_data_t *event_data) {
    if (event_data->event_status == Reading) {
        read_request(event_data);
    } else if (event_data->event_status == Writing) {
        write_response(event_data);
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

    server_event.events = EPOLLIN | EPOLLET;
    server_event.data.fd = server_socket_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket_fd, &server_event) == -1) {
        std::cerr << RED << "Error while adding socket to epoll: " << strerror(errno) << RESET << std::endl;
        exit(EXIT_FAILURE);
    }

    struct epoll_event epoll_events[MAX_EPOLL_EVENTS];
    while (1) {
        int nfds = epoll_wait(epoll_fd, epoll_events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1) {
            std::cerr << RED << "Error while waiting for epoll events: " << strerror(errno) << RESET << std::endl;
            continue;
        }

        for (int i = 0; i < nfds; i++) {
            if (epoll_events[i].data.fd == server_socket_fd) {
                struct sockaddr_in client_addr = {};
                socklen_t client_addr_len = sizeof(client_addr);

                int client_fd = accept(server_socket_fd, (struct sockaddr *) &client_addr, &client_addr_len);

                if (client_fd < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    } else {
                        std::cerr << RED << "Error while accepting client: " << strerror(errno) << RESET << std::endl;
                        break;
                    }
                }

                set_non_blocking(client_fd);

                event_data_t *event_data = new event_data_t;
                event_data->header = "";
                event_data->client_fd = client_fd;
                event_data->read_bytes = 0;
                event_data->event_status = Reading;

                request_event.data.ptr = event_data;
                request_event.events = EPOLLIN;

                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &request_event) == -1) {
                    std::cerr << RED << "Error while adding client to epoll: " << strerror(errno) << RESET << std::endl;
                    continue;
                }
            } else {
                event_data_t *event_data = (event_data_t *) epoll_events[i].data.ptr;
                process_event(event_data);
                switch (event_data->event_status) {
                    case Reading:
                        request_event.events = EPOLLIN;
                        request_event.data.ptr = event_data;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event_data->client_fd, &request_event) < 0) {
                            std::cerr << RED << "Error while adding reading event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Writing:
                        request_event.events = EPOLLOUT;
                        request_event.data.ptr = event_data;
                        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, event_data->client_fd, &request_event) < 0) {
                            std::cerr << RED << "Error while adding writing event to epoll: " << strerror(errno)
                                      << RESET << std::endl;
                            continue;
                        }
                        break;
                    case Ended:
                        if (event_data->file != NULL) {
                            fclose(event_data->file);
                            event_data->file = NULL;
                        }
                        close(event_data->client_fd);
                        delete event_data;
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
    int server_socket_fd = create_server_socket(8080 + (rand() % 10));
    set_non_blocking(server_socket_fd);
    io_multiplexing_event_loop(server_socket_fd);

    return EXIT_SUCCESS;
}