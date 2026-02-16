#include "TcpSocket.hpp"

#include "../../error/error.hpp"
#include "../../logger/logger.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ShiraNet::Sockets::TcpSocket::TcpSocket(int Domain) : Socket(Domain, SOCK_STREAM, IPPROTO_TCP) {

}

ShiraNet::Sockets::TcpSocket::TcpSocket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress) : Socket(SocketID, Domain, Type, Protocol, SocketAddress) {

}

void ShiraNet::Sockets::TcpSocket::connect(char* ServerIP, in_port_t ServerPort) {
    addStringIPToAddressInfo(ServerIP, std::to_string(ServerPort));
    socketAddress.sin_port = htons(ServerPort);
    socketAddress.sin_family = domain;

    if (::connect(socketID, (struct sockaddr *) &socketAddress, sizeof(socketAddress)) < 0) {
        throw "FUCK!"; // SHIRANET::ERROR
    }
}

void ShiraNet::Sockets::TcpSocket::bind(in_port_t ServerPort, in_addr_t ServerIP) {
    socketAddress.sin_addr.s_addr = htonl(ServerIP);
    socketAddress.sin_port = htons(ServerPort);
    socketAddress.sin_family = domain;

    if (::bind(socketID, (struct sockaddr* ) &socketAddress, sizeof(socketAddress)) < 0) {
        Logger::error("Socket bind failed");
        throw Exception(ErrorCode::BindFailed, "Failed to bind socket", errno);
    }

    Logger::debug("Socket binded successfully");
}

void ShiraNet::Sockets::TcpSocket::listen(int maxPendingRequest) {
    if (::listen(socketID, maxPendingRequest) < 0) {
        Logger::error("Socket listen failed");
        throw Exception(ErrorCode::ListenFailed, "Failed to listen with socket", errno);
    }
}

ShiraNet::Sockets::TcpSocket ShiraNet::Sockets::TcpSocket::getClientConnection() {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    int clientSocket = ::accept(socketID, (struct sockaddr *) &clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        Logger::error("Failed to accept socket");
        throw Exception(ErrorCode::AcceptFailed, "Failed to accept socket", errno);
    }
    
    return TcpSocket{clientSocket, domain, type, protocol, socketAddress};
}