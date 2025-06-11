#include "Connection.hpp"


Connection::Connection(CatNet::TCPSocket& socket)
    : socket(std::move(socket)), id(s_uniqueIDCount++)
{}
