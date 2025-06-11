#ifndef H_BUFFER_VIEW
#define H_BUFFER_VIEW

#include "TCPSocket.hpp"
#include <string>
#include <cstdint>

namespace CatNet
{

    class BufferView
    {

    public:
        BufferView(int8_t* buffer, std::size_t bufferSize);

        BufferView& write_int8(int8_t value);
        BufferView& write_int16(int16_t value);
        BufferView& write_int32(int32_t value);
        BufferView& write_string(const std::string& string);

        std::size_t write(const TCPSocket& socket, std::size_t bytes);
        std::size_t writeable_bytes() const;

        int8_t      read_int8();
        int32_t     read_int32();
        std::string read_string();

        std::size_t read(const TCPSocket& socket, std::size_t bytes);
        std::size_t readable_bytes() const;

    protected:
        void write(const void* source, size_t sizeInBytes);
        void read(void* destination, size_t sizeInBytes);

        int8_t* _buffer;
        std::size_t _readOffset;
        std::size_t _writeOffset;
        std::size_t _limit;
    };

}

#endif
