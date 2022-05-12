#ifndef LAB4_EVENTS_DB_H
#define LAB4_EVENTS_DB_H

#include <string>
#include <chrono>
#include <time.h>
#include <iostream>
#include <fstream>
#include <mutex>
//#include "mysql.h"
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
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

        /*MYSQL* mysql = mysql_init(0);

        mysql_real_connect(mysql, "127.0.0.1", "root", "123456", "mydb", 3306, NULL, 0);

        MYSQL_STMT* stmt = mysql_stmt_init(mysql);
        MYSQL_BIND    bind[4];
        string query = "INSERT INTO events (client_name, type, time, payload) VALUES (?, ?, ?, ?)";

        mysql_stmt_prepare(stmt, query.c_str(), query.size());

        bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
        bind[0].buffer = (char*)event.clientName.c_str();
        bind[0].buffer_length = event.clientName.size();
        bind[0].is_null = 0;
        bind[0].length = reinterpret_cast<unsigned long *>(event.clientName.size());

        bind[1].buffer_type = MYSQL_TYPE_SHORT;
        bind[1].buffer = (char*)&event.type;
        bind[1].buffer_length = sizeof(event.type);
        bind[1].is_null = 0;

        auto unixTime = static_cast<long>(event.time);

        bind[2].buffer_type = MYSQL_TYPE_LONG;
        bind[2].buffer = (char*)&unixTime;
        bind[2].buffer_length = sizeof(unixTime);
        bind[2].is_null = 0;

        bind[3].buffer_type = MYSQL_TYPE_STRING;
        bind[3].buffer = (char*)event.payload.c_str();
        bind[3].buffer_length = event.payload.size();
        bind[3].is_null = 0;
        bind[3].length = reinterpret_cast<unsigned long *>(event.payload.size());

        if (mysql_stmt_bind_param(stmt, bind))
        {
            fprintf(stderr, " mysql_stmt_bind_param() failed\n");
            fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            exit(0);
        }

        if (mysql_stmt_execute(stmt))
        {
            fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
            fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
            exit(1);
        }

        mysql_stmt_close(stmt);*/

        sql::Driver* driver;
        sql::Connection* connection;
        sql::PreparedStatement* pstmt;

        driver = get_driver_instance();

        try
        {
            auto debug1 = driver->getMajorVersion(); // 8
            auto debug2 = driver->getMinorVersion(); // 0
            auto debug3 = driver->getPatchVersion(); // 29
            auto debug4 = driver->getName(); // ""

            auto str1 = sql::SQLString("tcp://127.0.0.1:3306");
            auto str2 = sql::SQLString("root");
            auto str3 = sql::SQLString("123456");

            /*sql::ConnectOptionsMap m;

            m.try_emplace("hostName", "tcp://127.0.0.1:3306/mydb");
            m.try_emplace("userName", "root");
            m.try_emplace("password", "123456");*/

            // Exception 0xe06d7363 encountered at address 0x7ffe7f0e466c
            connection = driver->connect(str1, str2, str3);
        }
        catch (sql::SQLException e)
        {
            cout << "Could not connect to database. Error message: " << e.what() << endl;
            system("pause");
            exit(1);
        }

        connection->setSchema("mydb");

        pstmt = connection->prepareStatement("INSERT INTO events (client_name, type, time, payload) VALUES (?, ?, ?, ?)");
        pstmt->setString(0, event.clientName);
        pstmt->setInt(1, event.type);
        pstmt->setDateTime(2, to_string(event.time));
        pstmt->setString(3, event.payload);

        if (!pstmt->execute())
        {

        }

        this->_mutex->unlock();
    }
};

#endif //LAB4_CLIENTS_DB_H
