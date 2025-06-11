#ifndef H_TCPSOCKET
#define H_TCPSOCKET

#include <cstdint>
#include <optional>

namespace CatNet
{

    struct TCPSocket
    {
        TCPSocket(int descriptor);
        ~TCPSocket();

        TCPSocket(TCPSocket&& other);
        TCPSocket& operator=(TCPSocket&& other);

        TCPSocket(const TCPSocket& other) = delete;
        TCPSocket& operator=(const TCPSocket& other) = delete;

        bool close();

        bool bind(const char *ip, uint16_t port);
        bool listen(int backlog);

        bool connect(const char *ip, uint16_t port);
        std::optional<TCPSocket> accept() const ;

        std::size_t send(void* source, std::size_t bytes) const;
        std::size_t receive(void* destination, std::size_t bytes) const;

        bool setBlocking(bool isBlocking);

        static std::optional<TCPSocket> Create();

        int descriptor;
    };
}

#endif
