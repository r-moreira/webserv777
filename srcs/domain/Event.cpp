//
// Created by rmore on 09/04/2023.
//

#include "../../includes/domain/Event.h"

Event::Event(int client_fd) {
    this->_client_fd = client_fd;
    this->_event_status = Reading;
    this->_request = Request();
    this->_read_bytes = 0;
    this->_read_left = 0;
    this->_read_buffer = "";
    this->_file_path = "";
    this->_write_iteration = 0;
    this->_file = NULL;
}

Event::~Event() {
    if (_file != NULL) {
        fclose(_file);
    }
    close(this->getClientFd());
}

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

int Event::open_file() {
    if (this->getFile() == NULL) {
        this->setFilePath("./public" + this->getRequest().getUri());

        FILE *fptr;
        fptr = fopen(this->getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            std::cerr << RED << "Error while opening file: " << this->getFilePath() << " " << strerror(errno) << RESET << std::endl;
            //return error page, end connection
            this->setEventStatus(Ended);
            return -1;
        }
        this->setFile(fptr);
        //Por algum motivo readbytes precisa ser inicializado neste momento
        this->setReadBytes(0);
    }
    return 1;
}

void Event::write_response() {
    UrlParser urlParser;

    struct stat file_stat;
    char buffer[30720];

    if (open_file() == -1)
        return;

    int fd = fileno(this->getFile());
    if (fd < 1) {
        std::cerr << RED << "Error while getting file descriptor: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
    }

    fstat(fd, &file_stat);

    if (this->getWriteIteration() == 0) {
        std::string headers = getHeaders(this->getFilePath(), file_stat.st_size);
        std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

        if (send(this->getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
            std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET
                      << std::endl;
            //return error page, end connection
        }

        std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
    }

    size_t read_size;
    if (file_stat.st_size > 30720) {
        read_size = this->getReadLeft() > 30720 ? 30720 : this->getReadLeft();
    } else {
        read_size = file_stat.st_size;
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t it_read_bytes = fread(buffer, 1, read_size, this->getFile());

    this->setReadBytes(this->getReadBytes() + it_read_bytes);
    std::cout << YELLOW << "Readed Data Size: " << it_read_bytes << RESET << std::endl;

    this->setReadLeft(file_stat.st_size - this->getReadBytes());
    std::cout << YELLOW << "Read Left: " << this->getReadLeft() << RESET << std::endl;

    long bytes_sent = send(this->getClientFd() , buffer, it_read_bytes, 0 );
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
        this->setEventStatus(Ended);
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;

    this->setWriteIteration(this->getWriteIteration() + 1);
    if (this->getReadLeft() <= 0) {
        this->setEventStatus(Ended);
    }
}

Request Event::parse_request() {

    std::cout << MAGENTA << "Request Data:\n " << this->getReadBuffer() << RESET << std::endl;

    const char *buffer = this->getReadBuffer().c_str();

    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(this->_request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << _request.inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        //Return error page, end connection
    }
    this->setEventStatus(Writing);
    return _request;
}

void Event::read_request() {
    char buffer[READ_BUFFER_SIZE] = {};
    Request request;

    long bytes_read = read(this->getClientFd(), buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        this->setEventStatus(Ended);
        return;
    }

    this->setReadBytes(this->getReadBytes() + bytes_read);
    std::string read_buffer = this->getReadBuffer();
    this->setReadBuffer(read_buffer.append(buffer));

    std::cout << YELLOW << "Read " << this->getReadBytes() << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << buffer << RESET << std::endl;

    if (buffer[READ_BUFFER_SIZE - 1] == 0) {
        request = parse_request();
    }
    this->setRequest(request);
}

void Event::process_event() {
    if (this->getEventStatus() == Reading) {
        read_request();
    } else if (this->getEventStatus() == Writing) {
        write_response();
    }
}


int Event::getWriteIteration() const {
    return _write_iteration;
}

void Event::setWriteIteration(int writeIteration) {
    _write_iteration = writeIteration;
}

int Event::getClientFd() const {
    return _client_fd;
}

void Event::setClientFd(int clientFd) {
    _client_fd = clientFd;
}

const std::string &Event::getFilePath() const {
    return _file_path;
}

void Event::setFilePath(const std::string &filePath) {
    _file_path = filePath;
}

size_t Event::getReadBytes() const {
    return _read_bytes;
}

void Event::setReadBytes(size_t readBytes) {
    _read_bytes = readBytes;
}

size_t Event::getReadLeft() const {
    return _read_left;
}

void Event::setReadLeft(size_t readLeft) {
    _read_left = readLeft;
}

const std::string &Event::getReadBuffer() const {
    return _read_buffer;
}

void Event::setReadBuffer(const std::string &readBuffer) {
    _read_buffer = readBuffer;
}

const Request &Event::getRequest() const {
    return _request;
}

void Event::setRequest(const Request &request) {
    Event::_request = request;
}

event_status_t Event::getEventStatus() const {
    return _event_status;
}

void Event::setEventStatus(event_status_t eventStatus) {
    _event_status = eventStatus;
}

FILE *Event::getFile() const {
    return _file;
}

void Event::setFile(FILE *file) {
    Event::_file = file;
}
