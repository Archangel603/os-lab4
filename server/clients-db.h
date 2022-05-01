#ifndef LAB4_CLIENTS_DB_H
#define LAB4_CLIENTS_DB_H

#include <string>
#include <chrono>
#include <time.h>
#include <iostream>
#include <fstream>
#include <mutex>
#include "../shared/pipe-message.h"

using namespace std;

struct Client {
    string name;
    time_t connectedAt;
};

class ClientsDb {

public:

    explicit ClientsDb() {
        this->_mutex = new mutex();

        if (!this->dbExists()) {
            this->initDb();
        }
    }

    void addClient(PipeMessage* message) {
        auto now = chrono::system_clock::now();
        auto now_t = chrono::system_clock::to_time_t(now);

        Client client = { message->name, now_t };

        this->appendClientToDb(client);
    }

private:
    mutex* _mutex;
    char* _dbFile = "clients-db.txt";

    bool dbExists() {
        ifstream f(this->_dbFile);
        return f.good();
    }

    void initDb() {
        this->_mutex->lock();

        ofstream db(this->_dbFile, std::ios_base::app);
        db << "Client name" << "||" << "ConnectedAt" << endl;
        db.close();

        this->_mutex->unlock();
    }

    void appendClientToDb(Client& client) {
        this->_mutex->lock();

        ofstream db(this->_dbFile, std::ios_base::app);
        db << client.name << "||" << client.connectedAt << endl;
        db.close();

        this->_mutex->unlock();
    }
};

#endif //LAB4_CLIENTS_DB_H
