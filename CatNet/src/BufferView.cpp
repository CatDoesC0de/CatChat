#include "BufferView.hpp"

#include <cstring>
#include <arpa/inet.h>
#include <iostream>

namespace CatNet
{

    BufferView::BufferView(Buffer &buffer)
        : 
        m_buffer(buffer.get()), 
        m_readIndex(0), 
        m_readLimit(buffer.size())
    {}

    int32_t BufferView::int32()
    {
        int32_t value;
        read(&value, sizeof(int32_t));
        return ntohl(value);
    }

    int16_t BufferView::int16()
    {
        int16_t value;
        read(&value, sizeof(int16_t));
        return ntohs(value);
    }

    std::string BufferView::string()
    {
        int32_t stringLength = int32();

        //std::cout << "Incoming String Length: " << stringLength << '\n';

        if (m_readIndex + stringLength > m_readLimit)
        {
            //TODO: Overflow exception
        }

        std::string string((char*) m_buffer + m_readIndex, stringLength);
        m_readIndex += stringLength;

        return string;
    }

    void BufferView::read(void *destination, size_t sizeInBytes)
    {
        if (m_readIndex + sizeInBytes > m_readLimit)
        {
            //TODO: Overflow exception
        }

        std::memcpy(destination, m_buffer + m_readIndex, sizeInBytes);
        m_readIndex += sizeInBytes;
    }
}
