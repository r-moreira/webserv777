//
// Created by rmore on 10/04/2023.
//

#include "../../includes/domain/Response.h"

Response::Response(Event &event): _event(event), _file(event), _read(event), _write(event) {}

Response::~Response() {}

void Response::send_response_file() {
    _file.open_file();
    _read.read_file();
    _write.write_file_response_headers();
    _write.write_requested_file();
}

//TODO:: Checar se a pagina de diretório está configurada:
// caso exista retorna-la,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::send_is_directory_response() {
    std::cout << MAGENTA << "Send directory error response" << RESET << std::endl;

    // if existe uma pagina de erro de diretório configurada
    // read_error_configured_page(path_da_config_da_pagina_diretório);
    // _write.write_error_headers();
    // _write.write_configured_error_page();
    // else

    _write.write_error_headers();
    _write.write_default_error_page();

    _event.setEventStatus(Ended);
}

//TODO:: Checar se existem paginas de erro padrão configuradas:
// caso exista retorna-las,
// caso contratio, retornar paginas de erro padrão do webserv
void Response::send_error_response() {
    // if existe uma pagina de erro configurada para o erro atual
    // read_error_configured_page(path_da_config_da_pagina_de_erro)
    // _write.write_error_headers();
    // _write.write_configured_error_page();

    // else
    _write.write_error_headers();
    _write.write_default_error_page();

    _event.setEventStatus(Ended);
}
