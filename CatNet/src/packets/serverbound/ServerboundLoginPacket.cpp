#include "CatNet/packets/serverbound/ServerboundLoginPacket.hpp"

namespace CatNet
{

    void ServerboundLoginPacket::encode(CatNet::BufferView &writer) const
    {
        int32_t encodedSize = PACKET_HEADER_LENGTH + encoded_string_size(requestedUsername);
        writer.write_int32(encodedSize);
        writer.write_int8(id());
        writer.write_string(requestedUsername);
    }

    ServerboundLoginPacket ServerboundLoginPacket::decode(BufferView& reader)
    {
        std::string requestedUsername = reader.read_string();   
        ServerboundLoginPacket decodedPacket = {};
        decodedPacket.requestedUsername = std::move(requestedUsername);
        return decodedPacket;
    }

}
