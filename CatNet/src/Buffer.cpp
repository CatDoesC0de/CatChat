#include "Buffer.hpp"

#include "TCPSocket.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

namespace CatNet
{
    Buffer::Buffer(unsigned int sizeBytes)
        : 
        m_buffer(std::make_unique<uint8_t[]>(sizeBytes)),
        m_capacity(sizeBytes),
        m_limit(0)
    {}

    void Buffer::write(const void *data, size_t sizeInBytes)
    {
        if (m_limit + sizeInBytes > m_capacity)
        {
            // TODO: Overflow exception.
        }

        std::memcpy(m_buffer.get() + m_limit, data, sizeInBytes);
        m_limit += sizeInBytes;
    }

    Buffer& Buffer::int16(int16_t value)
    {
        value = htons(value);
        write(&value, sizeof(int16_t));
        return *this;
    }

    Buffer& Buffer::int32(int32_t value)
    {
        value = htonl(value);
        write(&value, sizeof(int32_t));
        return *this;
    }

    Buffer& Buffer::string(std::string &string)
    {
        int32(string.length());
        write(string.c_str(), string.length());
        return *this;
    }

    int Buffer::receive(const TCPSocket& socket)
    {
        int bytesReceived =  recv(socket.descriptor(), m_buffer.get(), m_capacity - m_limit, 0);
        m_limit += bytesReceived;
        return bytesReceived;
    }

    uint8_t *Buffer::get()
    {
        return m_buffer.get();
    }

    void Buffer::reserve(size_t capacityInBytes)
    {
        if (capacityInBytes <= m_capacity)
        {
            m_capacity = capacityInBytes;
        }
        else
        {
            auto newAlloc = std::make_unique<uint8_t[]>(capacityInBytes);
            std::memcpy(newAlloc.get(), m_buffer.get(), m_limit);
            m_buffer = std::move(newAlloc);
            m_capacity = capacityInBytes;
        }
    }

    void Buffer::clear()
    {
        m_limit = 0;
    }

    unsigned int Buffer::size() const
    {
        return m_limit;
    }

    unsigned int Buffer::capacity() const
    {
        return m_capacity;
    }
}
