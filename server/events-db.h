#ifndef LAB4_EVENTS_DB_H
#define LAB4_EVENTS_DB_H

#include <string>
#include <chrono>
#include <time.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../shared/pipe-message.h"

using namespace std;

struct Event {
    string clientName;
    int type;
    time_t time;
    string payload;
};

class EventsDb {

public:

    explicit EventsDb() {
        this->_mutex = new mutex();

        if (!this->dbExists()) {
            this->initDb();
        }
    }

    void addEvent(PipeMessage* message) {
        auto now = chrono::system_clock::now();
        auto now_t = chrono::system_clock::to_time_t(now);

        Event event = { message->name, message->type, now_t, message->payload };

        this->appendEventToDb(event);
    }

private:
    mutex* _mutex;
    char* _dbFile = "events-db.txt";

    bool dbExists() {
        ifstream f(this->_dbFile);
        return f.good();
    }

    void initDb() {
        this->_mutex->lock();

        ofstream db(this->_dbFile, std::ios_base::app);

        db  << "ClientName" << "||"
            << "EventType" << "||"
            << "EventTime" << "||"
            << "Payload" << endl;

        db.close();

        this->_mutex->unlock();
    }

    void appendEventToDb(Event& event) {
        this->_mutex->lock();

        ofstream db(this->_dbFile, std::ios_base::app);
        db  << event.clientName << "||"
            << event.type << "||"
            << event.time << "||"
            << event.payload << endl;
        db.close();

        this->_mutex->unlock();
    }
};

#endif //LAB4_CLIENTS_DB_H
