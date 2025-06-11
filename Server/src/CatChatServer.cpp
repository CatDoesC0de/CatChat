#include "CatChatServer.hpp"
#include "Connection.hpp" 

#include "CatNet/TCPSocket.hpp"
#include "CatNet/Buffer.hpp"
#include "CatNet/packets/serverbound/ServerboundLoginPacket.hpp"

#include <iostream>
#include <sys/epoll.h>
#include <vector>
#include <unordered_map>

void disconnect_connection(
        std::unordered_map<ConnectionID, CatNet::Buffer>& buffers,
        std::unordered_map<int, Connection>& connections,
        Connection& connection,
        int epollHandle,
        std::string reason
)
{
    epoll_ctl(epollHandle, EPOLL_CTL_DEL, connection.socket.descriptor, nullptr);
    buffers.erase(connection.id);
    connections.erase(connection.socket.descriptor);
    std::cout << "[Disconnect]: " << reason << '\n';
}

void start_cat_chat_server(uint16_t port)
{
    auto socketOptional = CatNet::TCPSocket::Create();
    
    if (!socketOptional.has_value())
    {
        std::cerr << "Failed to create server socket!\n";
        return;
    }

    auto listeningSocket = std::move(socketOptional.value());

    if (!listeningSocket.bind("127.0.0.1", port))
    {
        std::cerr << "Failed to bind on port " << port << ". Is it already in use?\n";
        return;
    }

    if (!listeningSocket.listen(3))
    {
        std::cerr << "Failed to bind on port " << port << ". Is it already in use?\n";
        return;
    }

    std::cout << "Server listening on port " << port << '\n';

    listeningSocket.setBlocking(false);

    int epollHandle = epoll_create1(0);
    epoll_event serverEpoll = {};
    serverEpoll.events = EPOLLIN;
    serverEpoll.data.fd = listeningSocket.descriptor;
    epoll_ctl(epollHandle, EPOLL_CTL_ADD, listeningSocket.descriptor, &serverEpoll);

    std::vector<epoll_event> events;
    events.resize(1);

    std::unordered_map<ConnectionID, CatNet::Buffer> connectionBuffers; //Map used to assign Connection to packet process structure
    std::unordered_map<int, Connection> connections; //Socket descriptor to Connection
    while (true)
    {
        int pollCount = epoll_wait(epollHandle, events.data(), events.size(), 1);

        for (std::size_t i = 0; i < pollCount; i++)
        {
            epoll_event event = events[i];

            if (event.data.fd == listeningSocket.descriptor)
            {
                while (true) // Grab as many connections as we can
                {
                    auto clientOptional = listeningSocket.accept();

                    if (!clientOptional)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) //Need to wait
                        {
                            break;
                        }
                        
                        std::cerr << "Failed to accept incoming connection!\n" << errno << '\n';
                        break;
                    }
                    
                    CatNet::TCPSocket client = std::move(clientOptional.value());
                    client.setBlocking(false);

                    epoll_event clientEpollEvent = {};
                    clientEpollEvent.events = EPOLLIN;
                    clientEpollEvent.data.fd = client.descriptor;
                    epoll_ctl(epollHandle, EPOLL_CTL_ADD, client.descriptor, &clientEpollEvent);
                    events.resize(events.size() + 1);

                    Connection connection(client);
                    connections.emplace(connection.socket.descriptor, std::move(connection));

                    connectionBuffers.emplace(connection.id, CatNet::Buffer(1024 * 64));

                    std::cout << "Accepted Connection!\n";
                }
            }
            else
            {
                Connection& connection = connections.at(event.data.fd);
                CatNet::Buffer& buffer = connectionBuffers.at(connection.id);
                
                if (event.events & EPOLLIN)
                {
                    std::size_t bytesRead = buffer.write(connection.socket, buffer.writeable_bytes());

                    if (bytesRead == 0) //Graceful disconnect
                    {
                        disconnect_connection(
                                connectionBuffers, 
                                connections, 
                                connection, 
                                epollHandle,
                                "Left...");
                        continue;
                    }

                    if (bytesRead == -1)
                    {
                        disconnect_connection(
                                connectionBuffers, 
                                connections, 
                                connection, 
                                epollHandle,
                                "Error when reading from socket");
                        continue;
                    }
                
                    std::cout << "[Read]: " << bytesRead << " bytes\n";

                    int32_t packetLength = buffer.read_int32();
                    int8_t packetID = buffer.read_int8();

                    std::cout << "Packet Length: " << packetLength << '\n';
                    std::cout << "Packet ID: " << (int32_t) packetID << '\n';
                    
                    auto loginPacket = CatNet::ServerboundLoginPacket::decode(buffer);
                    std::cout << "Login with username: " << loginPacket.requestedUsername << '\n';
                }
            }
        }
    }
}
