#include "db.h"

using namespace std;
using namespace sql;

Db::Db(SQLString connectionString, SQLString user, SQLString password) {
    this->_driver = mariadb::get_driver_instance();
    this->_connectionString = connectionString;
    this->_dbProperties = {{"user", user}, {"password", password}};
}

void Db::addClient(Client &client) {
    auto connection = this->createConnection();
    auto stmt = connection->prepareStatement("insert into clients (client_name,connected_at) values (?,?)");

    stmt->setString(1, client.name);
    stmt->setBigInt(2, to_string(client.connectedAt));

    stmt->executeQuery();
}

void Db::addEvent(Event &event) {
    auto connection = this->createConnection();
    auto stmt = connection->prepareStatement("insert into events (client_name,event_type,event_time,payload) values (?,?,?,?)");

    stmt->setString(1, event.clientName);
    stmt->setInt(2, event.type);
    stmt->setBigInt(3, to_string(event.time));
    stmt->setString(4, event.payload);

    stmt->executeQuery();
}

unique_ptr<Connection> Db::createConnection() {
    return unique_ptr<Connection>(this->_driver->connect(this->_connectionString, this->_dbProperties));
}