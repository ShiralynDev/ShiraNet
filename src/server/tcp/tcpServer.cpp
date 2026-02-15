#include "tcpServer.hpp"


ShiraNet::Servers::TcpServer::TcpServer(int domain, int port, int maxClients) {
    serverSocket = new ShiraNet::Sockets::TcpSocket(domain);
    serverSocket->bind(port);
    serverSocket->listen(maxClients);
}

ShiraNet::Servers::TcpServer::~TcpServer() {
    delete serverSocket;
}

void ShiraNet::Servers::TcpServer::getConnection() {
    clientSockets.push_back(std::move(serverSocket->getClientConnection()));
}

ShiraNet::Sockets::TcpSocket& ShiraNet::Servers::TcpServer::getLastClient() {
    return clientSockets.back();
}