#pragma once

#include <string>

#include "Buffer.hpp"

namespace CatNet
{
    class BufferView
    {

    public:
        BufferView(Buffer &buffer);
        
        int32_t int32();
        int16_t int16();

        std::string string();

    private:
        void read(void *destination, size_t sizeInBytes);

        uint8_t *m_buffer;
        unsigned int m_readIndex;
        unsigned int m_readLimit;
    };
}