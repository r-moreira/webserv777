//
// Created by rmore on 08/05/2023.
//

#include "../../includes/domain/ErrorState.h"

ErrorState::ErrorState() {}

void ErrorState::throw_error_state(Event &event, event_http_status_enum_t status) {
    std::cout << YELLOW << "Ativando estado de erro para o status " << status << RESET << std::endl;

    event.setHttpStatus(status);
    event.setEventStatus(Writing);
    event.setEventSubStatus(WritingErrorResponse);
}

bool ErrorState::is_error_state(Event &event) {
   return event.getHttpStatus() != OK;
}

