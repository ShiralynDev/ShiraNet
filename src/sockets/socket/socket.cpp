#include "socket.hpp"

#include "../../error/error.hpp"
#include "../../logger/logger.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

#include <cstring>

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

ShiraNet::Sockets::Socket::~Socket() {
    close(socketID);
}

void ShiraNet::Sockets::Socket::addStringIPToAddressInfo(char* ServerIP) {
    int inetptonResult = inet_pton(domain, ServerIP, &socketAddress.sin_addr.s_addr);
    if (inetptonResult == 0) {
        // SHIRANET::ERROR invalid server addresss/IP 
    } else if (inetptonResult < 0) {
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
    receiveBuffer.data.resize(totalBytesReceived); // hopefully this is the same but whatever
    return receiveBuffer;
}

std::string ShiraNet::Sockets::Socket::getAddressInfoToStringIP() {
    char addressName[INET_ADDRSTRLEN];
    inet_ntop(domain, &socketAddress.sin_addr.s_addr, addressName, sizeof(addressName));
    return addressName;
}