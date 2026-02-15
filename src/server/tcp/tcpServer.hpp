#pragma once

#include "../../sockets/tcp/TcpSocket.hpp"

#include <vector>

// Servers are classes containing sockets with a generic implementation of accepting clients and keeping track of them, this is a higher level part of the library and if you do not like the Server implementation you can use xSocket.hpp and create your own implementation
namespace ShiraNet::Servers {

    class TcpServer {
        private:
            ShiraNet::Sockets::TcpSocket *serverSocket = nullptr;

        public:
            // creates the servers socket, binds it and listens
            TcpServer(int domain, int port, int maxClients);
            ~TcpServer();

            std::vector<ShiraNet::Sockets::TcpSocket> clientSockets {};

            void getConnection();
            ShiraNet::Sockets::TcpSocket& getLastClient();
    };

}