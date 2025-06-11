#ifndef H_PACKET
#define H_PACKET

#include "BufferView.hpp"

#define PACKET_HEADER_LENGTH sizeof(int8_t) //1 byte for packet ID

namespace CatNet
{
    inline std::size_t encoded_string_size(const std::string& string) { return sizeof(int32_t) + string.size(); }

    struct Packet 
    {
        virtual void encode(CatNet::BufferView &writer) const = 0;
    };
} 

#endif
