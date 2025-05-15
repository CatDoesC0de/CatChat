#pragma once

#include "CatNet/TCPSocket.hpp"

#include <poll.h>

enum PacketProcessState
{
    PACKET_SIZE, PACKET_PAYLOAD
};

struct Connection
{
    Connection(pollfd* _pollInfo, CatNet::TCPSocket _socket);

    pollfd* pollInfo;
    CatNet::TCPSocket socket;
    CatNet::Buffer buffer;
    PacketProcessState processState;
};

void poll(const CatNet::TCPSocket& listeningSocket);
