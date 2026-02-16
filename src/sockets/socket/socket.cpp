#include "socket.hpp"

#include "../../error/error.hpp"
#include "../../logger/logger.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>

#include <cstring>
#include <iostream>

ShiraNet::Sockets::Socket::Socket(int Domain, int Type, int Protocol) {
    domain = Domain;
    type = Type;
    protocol = Protocol;

    Logger::debug("Creating socket...");
    socketID = socket(domain, type, protocol);

    if (socketID < 0) {
        Logger::error("Socket creation failed");
        throw Exception(ErrorCode::SocketCreationFailed, "Failed to create socket", errno);
    }
    Logger::info("Socket created successfully");
}

ShiraNet::Sockets::Socket::Socket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress) {
    socketID = SocketID;
    domain = Domain;
    type = Type;
    protocol = Protocol;
    socketAddress = SocketAddress;
    Logger::info("Socket class created from existing socket");
}

ShiraNet::Sockets::Socket::Socket(Socket&& other) noexcept {
    socketID = other.socketID;
    domain = other.domain;
    type = other.type;
    protocol = other.protocol;
    socketAddress = other.socketAddress;

    other.socketID = -1;
}

ShiraNet::Sockets::Socket& ShiraNet::Sockets::Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        if (socketID >= 0) {
            close(socketID);
        }
        socketID = other.socketID;
        domain = other.domain;
        type = other.type;
        protocol = other.protocol;
        socketAddress = other.socketAddress;

        other.socketID = -1;
    }
    return *this;
}

ShiraNet::Sockets::Socket::~Socket() {
    if (socketID >= 0) {
        close(socketID);
    }
}

void ShiraNet::Sockets::Socket::addStringIPToAddressInfo(char* ServerIP, std::string PortString) {
    int inetptonResult = inet_pton(domain, ServerIP, &socketAddress.sin_addr.s_addr);
    if (inetptonResult == 0) {
        struct addrinfo addressCriteria{ 0 };
        addressCriteria.ai_family = AF_UNSPEC;
        addressCriteria.ai_socktype = type;
        addressCriteria.ai_protocol = protocol;
        struct addrinfo *addressList{ 0 }; 
        int returnValue = getaddrinfo(ServerIP, PortString.c_str(), &addressCriteria, &addressList);
        if (returnValue != 0) {
            ShiraNet::Logger::error("getaddrinfo() failed" + std::string(gai_strerror(returnValue)));
        }
        struct sockaddr_in* firstGottenAddress = reinterpret_cast<struct sockaddr_in*>(addressList->ai_addr);
        socketAddress.sin_addr.s_addr = firstGottenAddress->sin_addr.s_addr;
        freeaddrinfo(addressList);
    } else if (inetptonResult < 0) {
        std::cerr << "test2\n";
        // SHIRANET::ERROR something failed :shrug:
    }
}

void ShiraNet::Sockets::Socket::send(Buffer& buffer) {
    Logger::debug("Sending " + std::to_string(buffer.size) + " bytes");
    ssize_t numberOfBytes = ::send(socketID, buffer.data.c_str(), buffer.size, 0);

    if (numberOfBytes < 0) {
        Logger::error("Send failed");
        throw Exception(ErrorCode::SendFailed, "Failed to send data", errno);
    } else if (numberOfBytes != buffer.size) {
        Logger::warning("Partial send: " + std::to_string(numberOfBytes) + "/" + std::to_string(buffer.size) + " bytes");
        throw Exception(ErrorCode::PartialSend, "Sent partial data");
    }

    Logger::debug("Successfully sent " + std::to_string(numberOfBytes) + " bytes");
}

Buffer ShiraNet::Sockets::Socket::receive(unsigned int AmountOfBytesToRead) {
    Buffer receiveBuffer{AmountOfBytesToRead, ""};

    unsigned int totalBytesReceived = 0;
    receiveBuffer.data.resize(AmountOfBytesToRead);
    
    while (totalBytesReceived < AmountOfBytesToRead) {
        ssize_t bytesReceived = 0;
        bytesReceived = ::recv(socketID, &receiveBuffer.data[totalBytesReceived], AmountOfBytesToRead - totalBytesReceived, 0);
        if (bytesReceived < 0) {
            Logger::warning("Failed to receive data");
            throw Exception(ErrorCode::ReceiveFailed, "Failed to receive data", errno);
        } else if (bytesReceived == 0) {
            Logger::warning("Connection closed early");
            throw Exception(ErrorCode::ConnectionClosedEarly, "Connection closed early");
        }
        totalBytesReceived += bytesReceived;
    }

    auto pos = receiveBuffer.data.find('\0');
    if (pos != std::string::npos) {
        receiveBuffer.data.resize(pos);
    }
    return receiveBuffer;
}

std::string ShiraNet::Sockets::Socket::getAddressInfoToStringIP() {
    char addressName[INET_ADDRSTRLEN];
    inet_ntop(domain, &socketAddress.sin_addr.s_addr, addressName, sizeof(addressName));
    return addressName;
}