#ifndef H_CONNECTION
#define H_CONNECTION

#include "CatNet/TCPSocket.hpp"

typedef std::size_t ConnectionID;

struct Connection
{
    inline static std::size_t s_uniqueIDCount = 0;

    Connection(CatNet::TCPSocket& socket);

    ConnectionID id;
    CatNet::TCPSocket socket;
};

#endif
