//
// Created by rmore on 08/05/2023.
//

#include "../../includes/domain/ErrorState.h"

ErrorState::ErrorState() {}

void ErrorState::throw_error_state(Event &event, event_http_status_enum_t status) {
    std::cout << YELLOW << "Ativando estado de erro para o status " << status << RESET << std::endl;

    event.setErrorResponse(false);
    event.setHttpStatus(status);
    event.setEventStatus(Writing);
    event.setEventSubStatus(SendingErrorResponse);
}

bool ErrorState::is_error_state(Event &event) {
   return event.getHttpStatus() != OK && !event.isErrorResponse();
}


void ErrorState::normalize_error_state(Event &event) {

    std::cout << YELLOW << "Normalizando estado de erro" << RESET << std::endl;

    event.setHttpStatus(OK);
}