#ifndef LAB4_CLIENTS_DB_H
#define LAB4_CLIENTS_DB_H

#include <string>
#include <chrono>
#include <time.h>
#include "db.h"
#include "../shared/pipe-message.h"

using namespace std;

class ClientService {

public:

    explicit ClientService(Db* db);

    void addClient(PipeMessage* message);

private:
    Db* _db;
};

#endif //LAB4_CLIENTS_DB_H
