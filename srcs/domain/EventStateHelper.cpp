//
// Created by rmore on 08/05/2023.
//

#include "../../includes/domain/EventStateHelper.h"

EventStateHelper::EventStateHelper() {}

void EventStateHelper::throw_error_state(Event &event, event_http_status_enum_t status) {
    std::cout << YELLOW << "Ativando estado de erro para o status " << status << RESET << std::endl;

    event.setHttpStatus(status);
    event.setEventStatus(Writing);
    event.setEventSubStatus(WritingErrorHeaders);
}

bool EventStateHelper::is_error_state(Event &event) {
   return event.getHttpStatus() != OK;
}

