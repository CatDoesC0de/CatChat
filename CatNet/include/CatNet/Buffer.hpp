#pragma once

#include <string>
#include <cstdint>
#include <memory>

namespace CatNet
{

    class TCPSocket;

    class Buffer
    {

    public:
        Buffer(unsigned int sizeBytes);

        Buffer& int16(int16_t value);
        Buffer& int32(int32_t value);
        Buffer& string(std::string& string);

        int receive(const TCPSocket& socket);

        uint8_t *get();
        void reserve(size_t capacityInBytes);

        void clear();
        unsigned int size() const;
        unsigned int capacity() const;

    private:
        void write(const void *data, size_t sizeInBytes);

        std::unique_ptr<uint8_t[]> m_buffer;

        unsigned int m_capacity;
        unsigned int m_limit;
    };

}
