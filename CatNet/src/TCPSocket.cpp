#include "TCPSocket.hpp"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

sockaddr_in buildAddressInfo(const char *ip, uint16_t port)
{
    sockaddr_in addressInfo = {};

    addressInfo.sin_family = AF_INET;
    addressInfo.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addressInfo.sin_addr.s_addr);

    return addressInfo;
}

namespace CatNet
{

    TCPSocket::TCPSocket(int descriptor)
        : descriptor(descriptor)
    {
    }

    TCPSocket::~TCPSocket()
    {
        this->close();
    }

    TCPSocket::TCPSocket(TCPSocket&& other)
        : descriptor(other.descriptor)
    {
        other.descriptor = -1;
    }

    TCPSocket& TCPSocket::operator=(TCPSocket&& other)
    {
        if (this != &other)
        {
            if (descriptor != -1)
            {
                ::close(descriptor);
            }

            descriptor = other.descriptor;
            other.descriptor = -1;
        }

        return *this;
    }

    bool TCPSocket::bind(const char *ip, uint16_t port)
    {
        sockaddr_in addressInfo = buildAddressInfo(ip, port);
        return ::bind(descriptor, (sockaddr *)&addressInfo, sizeof(addressInfo)) == 0;
    }

    bool TCPSocket::listen(int backlog)
    {
        return ::listen(descriptor, backlog) == 0;
    }

    bool TCPSocket::connect(const char *ip, uint16_t port)
    {
        sockaddr_in addressInfo = buildAddressInfo(ip, port);
        return ::connect(descriptor, (sockaddr*) &addressInfo, sizeof(addressInfo)) == 0;
    }

    bool TCPSocket::close()
    {
        if (descriptor == -1)
        {
            return true;
        }

        return ::close(descriptor) == 0;
    }

    std::optional<TCPSocket> TCPSocket::accept() const
    {
        int acceptedDescriptor = ::accept(descriptor, nullptr, nullptr);

        if (acceptedDescriptor == -1)
        {
            return std::nullopt;
        }

        return TCPSocket(acceptedDescriptor);
    }

    std::size_t TCPSocket::send(void* source, std::size_t bytes) const
    {
        return ::send(descriptor, source, bytes, 0);
    }


    std::size_t TCPSocket::receive(void* destination, std::size_t bytes) const
    {
        return ::recv(descriptor, destination, bytes, 0);
    }

    bool TCPSocket::setBlocking(bool isBlocking)
    {
        return ::fcntl(descriptor, F_SETFL, isBlocking ? ~O_NONBLOCK : O_NONBLOCK) == 0;
    }

    std::optional<TCPSocket> TCPSocket::Create()
    {
        int descriptor = socket(AF_INET, SOCK_STREAM, 0);

        if (descriptor == -1)
        {
            return std::nullopt;
        }

        return TCPSocket(descriptor);
    }
}
