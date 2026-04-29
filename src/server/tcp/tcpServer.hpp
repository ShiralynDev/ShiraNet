#pragma once

#include "../../sockets/tcp/tcpSocket.hpp"

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

// Servers are classes containing sockets with a generic implementation of accepting clients and keeping track of them, this is a higher level part of the library and if you do not like the Server implementation you can use xSocket.hpp and create your own implementation
namespace ShiraNet::Servers {

    class TcpServer {
    private:
        using ClientHandlerCallback = std::function<void(std::shared_ptr<ShiraNet::Sockets::TcpSocket>, ShiraNet::Servers::TcpServer*)>;

        ShiraNet::Sockets::TcpSocket* serverSocket = nullptr;
        std::vector<std::shared_ptr<ShiraNet::Sockets::TcpSocket>> clients;
        std::mutex clientsMutex;
        std::thread serverThread;

    public:
        // creates the servers socket, binds it and listens
        TcpServer(int Domain, int Port, int MaxClients);
        ~TcpServer();

        void removeClient(std::shared_ptr<ShiraNet::Sockets::TcpSocket> Client);
        void getConnection(); // Accepts any incomming connection, adds it's socket to the clients list
        void getConnection(ClientHandlerCallback Callback);
        void getConnections(); // Accepts any incoming connections in a loop and is ran on the serverThread;
        void getConnections(ClientHandlerCallback Callback);
        std::shared_ptr<ShiraNet::Sockets::TcpSocket> getClient(int i);
        void sendToAll(ShiraNet::NetworkData::Message Message);
        void sendToAllExcept(ShiraNet::NetworkData::Message Message, std::shared_ptr<ShiraNet::Sockets::TcpSocket> Client);
    };

}