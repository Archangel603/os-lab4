
#ifndef LAB4_DB_H
#define LAB4_DB_H

#include <mariadb/conncpp.hpp>
#include <chrono>
#include <time.h>
#include <string>

using namespace std;
using namespace sql;

struct Event {
    string clientName;
    int type;
    time_t time;
    string payload;
};

struct Client {
    string name;
    time_t connectedAt;
};

class Db {
public:
    Db(SQLString connectionString, SQLString user, SQLString password);

    void addClient(Client& client);

    void addEvent(Event& event);

private:
    Driver* _driver;
    SQLString _connectionString;
    Properties _dbProperties;

    unique_ptr<sql::Connection> createConnection();
};

#endif