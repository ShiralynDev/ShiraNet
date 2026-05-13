#include "tcpSocket.hpp"

#include "../../error/error.hpp"
#include "../../logger/logger.hpp"

#include <cstring>

ShiraNet::Sockets::TcpSocket::TcpSocket()
  : Socket() {
}

ShiraNet::Sockets::TcpSocket::TcpSocket(int Domain)
  : Socket(Domain, SOCK_STREAM, IPPROTO_TCP) {
}

ShiraNet::Sockets::TcpSocket::TcpSocket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress)
  : Socket(SocketID, Domain, Type, Protocol, SocketAddress) {
}

void ShiraNet::Sockets::TcpSocket::connect(const std::string& ServerIP, const in_port_t& ServerPort) {
    addStringIPToAddressInfo(ServerIP, std::to_string(ServerPort));
    socketAddress.sin_port = htons(ServerPort);
    socketAddress.sin_family = domain;

    if (::connect(socketID, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        throw "FUCK!"; // SHIRANET::ERROR
    }
}

void ShiraNet::Sockets::TcpSocket::bind(const in_port_t& ServerPort, const uint32_t& ServerIP) {
    socketAddress.sin_addr.s_addr = ServerIP;
    socketAddress.sin_port = htons(ServerPort);
    socketAddress.sin_family = domain;

    if (::bind(socketID, (struct sockaddr*)&socketAddress, sizeof(socketAddress)) < 0) {
        Logger::error("Socket bind failed: " + std::string(strerror(errno)));
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

    int clientSocket = ::accept(socketID, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        Logger::info("Failed to accept socket");
        return TcpSocket{ AF_UNSPEC };
    }

    return TcpSocket{ clientSocket, domain, type, protocol, socketAddress };
}