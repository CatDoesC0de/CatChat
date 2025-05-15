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
        : m_descriptor(descriptor)
    {
    }

    TCPSocket::~TCPSocket()
    {
        this->close();
    }

    TCPSocket::TCPSocket(TCPSocket&& other)
    {
        m_descriptor = other.m_descriptor;
        other.m_descriptor = -1;
    }

    void TCPSocket::operator=(TCPSocket&& other)
    {
        m_descriptor = other.m_descriptor;
        other.m_descriptor = -1;
    }

    int TCPSocket::descriptor() const
    {
        return m_descriptor;
    }

    bool TCPSocket::bind(const char *ip, uint16_t port)
    {
        sockaddr_in addressInfo = buildAddressInfo(ip, port);
        return ::bind(m_descriptor, (sockaddr *)&addressInfo, sizeof(addressInfo)) == 0;
    }

    bool TCPSocket::listen(int backlog)
    {
        return ::listen(m_descriptor, backlog) == 0;
    }

    bool TCPSocket::connect(const char *ip, uint16_t port)
    {
        sockaddr_in addressInfo = buildAddressInfo(ip, port);
        return ::connect(m_descriptor, (sockaddr*) &addressInfo, sizeof(addressInfo)) == 0;
    }

    bool TCPSocket::close()
    {
        if (m_descriptor == -1)
        {
            return true;
        }

        return ::close(m_descriptor) == 0;
    }

    std::optional<TCPSocket> TCPSocket::accept() const
    {
        int descriptor = ::accept(m_descriptor, nullptr, nullptr);

        if (descriptor == -1)
        {
            return std::nullopt;
        }

        return TCPSocket(descriptor);
    }

    int TCPSocket::send(Buffer& buffer)
    {
        return ::send(m_descriptor, buffer.get(), buffer.size(), 0);
    }

    bool TCPSocket::blocking(bool isBlocking)
    {
        return ::fcntl(m_descriptor, F_SETFL, isBlocking ? ~O_NONBLOCK : O_NONBLOCK) == 0;
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
