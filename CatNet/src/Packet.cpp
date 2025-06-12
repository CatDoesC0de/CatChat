#include "Packet.hpp"

namespace CatNet
{
    void encode_packet_with_header(const Packet& packet, BufferView& view)
    {
        view.set_write_offset(PACKET_HEADER_LENGTH); //Start packet encoding at PACKET_HEADER_LENGTH
        packet.encode(view);

        std::size_t encodedBytes = view.readable_bytes() - (PACKET_HEADER_LENGTH); //Get the packet size
        view.set_write_offset(0); //Go back to the front of the buffer
        view.write_int32(encodedBytes + sizeof(int8_t)); //Prefix the packet size (id + payload)
        view.write_int8(packet.id()); //ID afterwards
        // Buffer now looks like -> | Packet Size = (ID Size + Payload Size) | Packet ID | Payload |
        view.set_write_offset(encodedBytes + PACKET_HEADER_LENGTH); //Reset write offset
    }
}
