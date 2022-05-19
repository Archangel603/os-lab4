#include "event-service.h"

EventService::EventService(Db *db) {
    this->_db = db;
}

void EventService::addEvent(PipeMessage *message) {
    auto now = chrono::system_clock::now();
    auto now_t = chrono::system_clock::to_time_t(now);

    Event event = { message->name, message->type, now_t, message->payload };

    this->_db->addEvent(event);
}