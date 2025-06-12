#include "CatNet/packets/serverbound/ServerboundLoginPacket.hpp"

namespace CatNet
{

    void ServerboundLoginPacket::encode(CatNet::BufferView &writer) const
    {
        writer.write_string(requestedUsername);
    }


    int8_t ServerboundLoginPacket::id() const
    {
        return 0x00;
    }

    ServerboundLoginPacket ServerboundLoginPacket::decode(BufferView& reader)
    {
        std::string requestedUsername = reader.read_string();   
        ServerboundLoginPacket decodedPacket = {};
        decodedPacket.requestedUsername = std::move(requestedUsername);
        return decodedPacket;
    }

}
