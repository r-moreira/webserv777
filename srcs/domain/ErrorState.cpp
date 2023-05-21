//
// Created by rmore on 08/05/2023.
//

#include "../../includes/domain/ErrorState.h"

ErrorState::ErrorState() {}

void ErrorState::throw_error_state(Event &event, Event::HttpStatus::event_http_status status) {
    std::cout << YELLOW << "Ativando estado de erro para o status " << status << RESET << std::endl;

    event.setErrorResponse(false);
    event.setHttpStatus(status);
    event.setEventStatus(Event::Status::Writing);
    event.setEventSubStatus(Event::SubStatus::SendingErrorResponse);
}

bool ErrorState::is_error_state(Event &event) {
   return event.getHttpStatus() != Event::HttpStatus::OK && !event.isErrorResponse();
}