#include "client-service.h"

ClientService::ClientService(Db *db) {
    this->_db = db;
}

void ClientService::addClient(PipeMessage *message) {
    auto now = chrono::system_clock::now();
    auto now_t = chrono::system_clock::to_time_t(now);

    Client client = { message->name, now_t };

    this->_db->addClient(client);
}