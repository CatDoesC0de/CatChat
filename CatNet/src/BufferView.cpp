#include "BufferView.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

namespace CatNet
{
    BufferView::BufferView(int8_t* buffer, std::size_t bufferSize)
        : 
            _buffer(buffer),
            _limit(bufferSize),
            _readOffset(0),
            _writeOffset(0)
    {}

    void BufferView::write(const void *data, std::size_t sizeInBytes)
    {
        if (writeable_bytes() < sizeInBytes)
        {
            // TODO: Overflow exception.
        }

        std::memcpy(_buffer + _writeOffset, data, sizeInBytes);
        _writeOffset += sizeInBytes;
    }

    BufferView& BufferView::write_int8(int8_t value)
    {
        value = htons(value);
        write(&value, sizeof(int8_t));
        return *this;
    }

    BufferView& BufferView::write_int16(int16_t value)
    {
        value = htons(value);
        write(&value, sizeof(int16_t));
        return *this;
    }

    BufferView& BufferView::write_int32(int32_t value) 
    { 
        value = htonl(value);
        write(&value, sizeof(int32_t));
        return *this;
    }

    BufferView& BufferView::write_string(const std::string &string) 
    {
        write_int32(string.length());
        write(string.c_str(), string.length());
        return *this;
    }

    std::size_t BufferView::write(const TCPSocket& socket, std::size_t bytes)
    {
        int bytesReceived = socket.receive(_buffer + _writeOffset, bytes);
        _writeOffset += bytesReceived;
        return bytesReceived;
    }

    std::size_t BufferView::writeable_bytes() const
    {
        return _limit - _writeOffset;
    }

    void BufferView::set_write_offset(std::size_t offset)
    {
        _writeOffset = offset;
    }

    void BufferView::read(void* destination, std::size_t sizeInBytes)
    {
        if (readable_bytes() < sizeInBytes)
        {
            //TODO: Overflow
        }

        std::memcpy(destination, _buffer + _readOffset, sizeInBytes);
        _readOffset += sizeInBytes;
    }

    int8_t BufferView::read_int8()
    {
        int8_t value =  *(_buffer + _readOffset);
        _readOffset += sizeof(int8_t);
        return value;
    }

    int32_t BufferView::read_int32() 
    {
        int32_t value;
        read(&value, sizeof(int32_t));
        value = ntohl(value);
        return value;
    }

    std::string BufferView::read_string()
    {
        int32_t stringLength = read_int32();
        std::string result((char*) (_buffer + _readOffset), stringLength);
        _readOffset += stringLength;
        return result;
    }

    void BufferView::set_read_offset(std::size_t offset)
    {
        _readOffset = offset;
    }

    std::size_t BufferView::send(const TCPSocket& socket, std::size_t bytes)
    {
        int bytesSent = socket.send(_buffer + _readOffset, bytes);
        _readOffset += bytesSent;
        return bytesSent;
    }

    std::size_t BufferView::readable_bytes() const
    {
        return _writeOffset - _readOffset;
    }
}
