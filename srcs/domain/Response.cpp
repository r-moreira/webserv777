//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event): _event(event), _file(event), _read(event), _write(event) {}

Response::~Response() {}

void Response::write_response_file() {
    _file.open_file();
    _read.read_file();
    _write.write_file_response_headers();
    write_requested_file();
}

void Response::write_requested_file() {
    if (ErrorState::is_error_state(this->_event)) return;

    long bytes_sent = send(_event.getClientFd() , _event.getFileReadChunkBuffer(), _event.getFileChunkReadBytes(), 0);
    if (bytes_sent < 0) {
        std::cerr << RED << "Error while writing to client: " << strerror(errno) << RESET << std::endl;
        _event.setEventStatus(Ended);
        return;
    }

    std::cout << YELLOW << "Transmitted Data Size " << bytes_sent << " Bytes." << RESET << std::endl;

    if (_event.getFileReadLeft() <= 0) {
        _event.setEventStatus(Ended);
        std::cout << GREEN << "File Transfer Complete." << RESET << std::endl;
    }
}

//TODO:: Checar se a pagina de diretório está configurada:
// caso exista retorna-la,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::write_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

    // if existe uma pagina de erro de diretório configurada
        // read_error_configured_page(path_da_config_da_pagina_diretório);
        // write_error_headers();
        // write_configured_error_page();
    // else

    _write.write_error_headers();
    write_default_error_page();

    _event.setEventStatus(Ended);
}

//TODO:: Checar se existem paginas de erro padrão configuradas:
// caso exista retorna-las,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::write_error_response() {
    // if existe uma pagina de erro configurada para o erro atual
        // read_error_configured_page(path_da_config_da_pagina_de_erro)
        // write_error_headers();
        // write_configured_error_page();

    // else
    _write.write_error_headers();
    write_default_error_page();

    _event.setEventStatus(Ended);
}


void Response::write_default_error_page() {
    std::ostringstream error_page;
    std::string html_tag_init = "<html><body><h1>";
    std::string html_message = "Webserv Error: ";
    long error_status_code = this->_event.getHttpStatus();
    std::string html_tag_end =  "</h1></body></html>";

    error_page << html_tag_init << html_message << error_status_code << html_tag_end;

    std::cout << CYAN << "Response Page:\n" << error_page.str() << RESET << std::endl;
    if (send(_event.getClientFd(), error_page.str().c_str(), error_page.str().size(), 0) < 0) {
        std::cerr << RED << "Error while writing error page to client: " << strerror(errno) << RESET << std::endl;
    }

    std::cout << GREEN << "Successfully sent error page to client" << RESET << std::endl;
}
