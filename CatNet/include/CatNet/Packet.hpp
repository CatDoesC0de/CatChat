#ifndef H_PACKET
#define H_PACKET

#include "BufferView.hpp"

#define PACKET_HEADER_LENGTH sizeof(int32_t) + sizeof(int8_t) //4 byte length prefix + 1 byte packet ID

namespace CatNet
{
    struct Packet 
    {
        virtual int8_t id() const = 0;
        virtual void encode(CatNet::BufferView& writer) const = 0;
    };

    void encode_packet_with_header(const Packet& packet, BufferView& view);
} 

#endif
