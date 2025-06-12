#ifndef H_SERVERBOUND_LOGIN_PACKET
#define H_SERVERBOUND_LOGIN_PACKET

#include "CatNet/Packet.hpp"
#include <string>

namespace CatNet
{
    struct ServerboundLoginPacket : public Packet
    {
        void encode(BufferView &writer) const override;
        int8_t id() const override;

        static ServerboundLoginPacket decode(BufferView& reader);

        std::string requestedUsername;
    };
}

#endif
