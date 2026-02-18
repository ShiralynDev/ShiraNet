#include "tcpServer.hpp"


ShiraNet::Servers::TcpServer::TcpServer(int Domain, int Port, int MaxClients) {
    serverSocket = new ShiraNet::Sockets::TcpSocket(Domain);
    serverSocket->bind(Port);
    serverSocket->listen(MaxClients);
}

ShiraNet::Servers::TcpServer::~TcpServer() {
    delete serverSocket;
}

int ShiraNet::Servers::TcpServer::getConnection() {
    clientSockets.push_back(std::move(serverSocket->getClientConnection()));
    return clientSockets.size() - 1;
}

ShiraNet::Sockets::TcpSocket& ShiraNet::Servers::TcpServer::getLastClient() {
    return clientSockets.back();
}