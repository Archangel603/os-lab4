#ifndef LAB4_EVENTS_DB_H
#define LAB4_EVENTS_DB_H

#include <string>
#include <chrono>
#include <time.h>
#include "db.h"
#include "../shared/pipe-message.h"

using namespace std;

class EventService {

public:

    explicit EventService(Db* db);

    void addEvent(PipeMessage* message);

private:
    Db* _db;
};

#endif //LAB4_CLIENTS_DB_H
