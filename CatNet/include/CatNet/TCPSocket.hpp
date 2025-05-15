#pragma once

#include <cstdint>
#include <optional>

#include "Buffer.hpp"

namespace CatNet
{

    class TCPSocket
    {
    public:
        TCPSocket(int descriptor);
        ~TCPSocket();

        TCPSocket(TCPSocket&& other);
        void operator=(TCPSocket&& other);

        TCPSocket(const TCPSocket& other) = delete;
        void operator=(const TCPSocket& other) = delete;

        bool close();

        int descriptor() const;

        bool bind(const char *ip, uint16_t port);
        bool listen(int backlog);

        bool connect(const char *ip, uint16_t port);
        std::optional<TCPSocket> accept() const ;

        int send(Buffer &buffer);

        bool blocking(bool isBlocking);

        static std::optional<TCPSocket> Create();

    private:
        int m_descriptor;
    };
}
