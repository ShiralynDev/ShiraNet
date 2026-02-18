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
            TcpServer(int Domain, int Port, int MaxClients);
            ~TcpServer();

            std::vector<ShiraNet::Sockets::TcpSocket> clientSockets {};

            int getConnection(); // Accepts any incomming connection, adds it's socket to the clientSockets list and return the position in the vector for this new socket
            ShiraNet::Sockets::TcpSocket& getLastClient();
    };

}