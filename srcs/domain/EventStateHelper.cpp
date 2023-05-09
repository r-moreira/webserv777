//
// Created by rmore on 08/05/2023.
//

#include "../../includes/domain/EventStateHelper.h"

EventStateHelper::EventStateHelper() {}

void EventStateHelper::error_state(Event &event, event_http_status_enum_t status) {
    event.setHttpStatus(status);
    event.setEventStatus(Writing);
    event.setEventSubStatus(WritingErrorHeaders);
}