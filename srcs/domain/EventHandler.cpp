//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/EventHandler.h"

EventHandler::EventHandler(Event &event) : _event(event) {}

EventHandler::~EventHandler() {}

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

std::string EventHandler::getHeaders(const std::string& file_path, size_t file_size) {
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

void EventHandler::open_file() {
    if (this->_event.getEventStatus() == Ended) return;

    if (this->_event.getFile() == NULL) {
        this->_event.setFilePath("./public" + this->_event.getRequest().getUri());

        FILE *fptr;
        fptr = fopen(this->_event.getFilePath().c_str(), "rb");
        if (fptr == NULL) {
            std::cerr << RED << "Error while opening file: " << this->_event.getFilePath() << " " << strerror(errno) << RESET << std::endl;
            //return error page, end connection
            this->_event.setEventStatus(Ended);
        }
        this->_event.setFile(fptr);
        //Por algum motivo readbytes precisa ser inicializado neste momento
        this->_event.setFileReadBytes(0);
    }

    struct stat file_stat;
    int fd = fileno(this->_event.getFile());
    if (fd < 1) {
        std::cerr << RED << "Error while getting file descriptor: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
    }

    fstat(fd, &file_stat);
    this->_event.setFileSize(file_stat.st_size);
    this->_event.setEventSubStatus(WritingResponseHeaders);
}

void EventHandler::upload_file() {
    if (this->_event.getEventStatus() == Ended) return;

    read_upload_file();

    write_upload_file();

}

void EventHandler::write_upload_file() {
    if (this->_event.getEventStatus() == Ended) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        //return error page, end connection
        _event.setEventStatus(Ended);
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;


    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Ended);

        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

void EventHandler::read_upload_file() {
    if (this->_event.getEventStatus() == Ended) return;

    size_t read_size;
    if (_event.getFileSize() > FILE_READ_CHUNK_SIZE) {
        read_size = _event.getFileReadLeft() > FILE_READ_CHUNK_SIZE ? FILE_READ_CHUNK_SIZE : _event.getFileReadLeft();
    } else {
        read_size = _event.getFileSize();
    }

    std::cout << YELLOW << "Read Data Size: " << read_size << RESET << std::endl;
    size_t chunk_bytes = fread((void *) this->_event.getFileReadChunkBuffer(), 1, read_size, _event.getFile());

    _event.setFileReadBytes(_event.getFileReadBytes() + chunk_bytes);
    std::cout << YELLOW << "Readed Data Size: " << chunk_bytes << RESET << std::endl;

    _event.setFileReadLeft(_event.getFileSize() - _event.getFileReadBytes());
    std::cout << YELLOW << "Read Left: " << _event.getFileReadLeft() << RESET << std::endl;

    this->_event.setFileChunkReadBytes(chunk_bytes);
}

void EventHandler::write_response_headers() {
    if (this->_event.getEventStatus() == Ended) return;

    std::string headers = getHeaders(_event.getFilePath(), _event.getFileSize());
    std::cout << CYAN << "Response Headers:\n" << headers << RESET << std::endl;

    if (send(_event.getClientFd(), headers.c_str(), headers.size(), 0) < 0) {
        std::cerr << RED << "Error while writing status header to client: " << strerror(errno) << RESET << std::endl;
        this->_event.setEventStatus(Ended);
        //return error page, end connection
    }

    std::cout << GREEN << "Successfully sent headers to client" << RESET << std::endl;
    this->_event.setEventSubStatus(UploadingFile);
}

void EventHandler::parse_request() {
    if (this->_event.getEventStatus() == Ended) return;

    std::cout << MAGENTA << "Request Data:\n " << this->_event.getRequestReadBuffer() << RESET << std::endl;

    const char *buffer = this->_event.getRequestReadBuffer().c_str();

    HttpRequestParser parser;

    HttpRequestParser::ParseResult result = parser.parse(_event._request, buffer, buffer + strlen(buffer));

    if (result == HttpRequestParser::ParsingCompleted) {
        std::cout << WHITE << "Parsed Request:\n" << _event.getRequest().inspect() << RESET << std::endl;
    } else {
        std::cerr << RED << "Parsing failed" << RESET << std::endl;
        //Return error page, end connection
        this->_event.setEventStatus(Ended);
    }
    this->_event.setEventStatus(Writing);

    // Por enquanto só vai ter um sub estado de leitura de arquivo, mas no futuro terá outros dependendo da funcionalidade
    this->_event.setEventSubStatus(OpeningFile);
}

void EventHandler::read_request() {
    if (this->_event.getEventStatus() == Ended) return;

    char buffer[READ_BUFFER_SIZE] = {};
    RequestInfo request;

    long bytes_read = read(this->_event.getClientFd(), buffer, READ_BUFFER_SIZE);

    if (bytes_read == -1) {
        std::cerr << RED << "Error while reading from client: " << strerror(errno) << RESET << std::endl;
        this->_event.setEventStatus(Ended);
    } else if (bytes_read == 0) {
        std::cout << YELLOW << "Client disconnected" << RESET << std::endl;
        this->_event.setEventStatus(Ended);
        return;
    }

    this->_event.setRequestReadBytes(this->_event.getRequestReadBytes() + bytes_read);
    std::string read_buffer = this->_event.getRequestReadBuffer();
    this->_event.setRequestReadBuffer(read_buffer.append(buffer));

    std::cout << YELLOW << "Read " << this->_event.getRequestReadBytes() << " bytes from client" << RESET << std::endl;
    std::cout << GREEN << "HTTP Request:\n" << buffer << RESET << std::endl;

    if (buffer[READ_BUFFER_SIZE - 1] == 0) {
        this->_event.setEventSubStatus(ParsingRequest);
    }
}

void EventHandler::process_event() {
    if (this->_event.getEventStatus() == Reading) {

        switch (this->_event.getEventSubStatus()) {
            case ReadingRequest: read_request();
            case ParsingRequest: parse_request();
                break;
            default:
                std::cerr << RED << "Invalid Reading Event Sub Status" << RESET << std::endl;
                break;
        }

    } else if (this->_event.getEventStatus() == Writing) {

        switch (this->_event.getEventSubStatus()) {
            case OpeningFile: open_file();
            case WritingResponseHeaders: write_response_headers();
            case UploadingFile: upload_file();
                break;
            default:
                std::cerr << RED << "Invalid Writing Event Sub Status" << RESET << std::endl;
                break;
        }

    }

    std::cout << "Event Status: " << this->_event.getEventStatus() << std::endl;
    std::cout << "Event Sub Status: " << this->_event.getEventSubStatus() << std::endl;
}


const Event &EventHandler::getEvent() const {
    return _event;
}

void EventHandler::setEvent(const Event &event) {
    this->_event = event;
}
