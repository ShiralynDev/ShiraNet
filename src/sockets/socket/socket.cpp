#include "socket.hpp"

#include "../../error/error.hpp"
#include "../../logger/logger.hpp"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <cstdint>
#include <cstring>
#include <iostream>

ShiraNet::Sockets::Socket::Socket() {
    isValid = false;
}

ShiraNet::Sockets::Socket::Socket(int Domain, int Type, int Protocol) {
    domain = Domain;
    type = Type;
    protocol = Protocol;
    initWinsock();

    Logger::debug("Creating socket...");
    socketID = ::socket(domain, type, protocol);

    if (socketID < 0) {
        Logger::error("Socket creation failed");
        isValid = false;
    }
    Logger::info("Socket created successfully");
    isValid = true;
}

ShiraNet::Sockets::Socket::Socket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress) {
    socketID = SocketID;
    domain = Domain;
    type = Type;
    protocol = Protocol;
    socketAddress = SocketAddress;
    Logger::info("Socket class created from existing socket");
    isValid = true;
}

ShiraNet::Sockets::Socket::Socket(Socket&& other) noexcept {
    socketID = other.socketID;
    domain = other.domain;
    type = other.type;
    protocol = other.protocol;
    socketAddress = other.socketAddress;
    isValid = other.isValid;

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
        isValid = other.isValid;

        other.socketID = -1;
    }
    return *this;
}

ShiraNet::Sockets::Socket::~Socket() {
    if (socketID >= 0) {
        close(socketID);
    }
}

void ShiraNet::Sockets::Socket::addStringIPToAddressInfo(const std::string& ServerIP, const std::string& PortString) {
    int inetptonResult = inet_pton(domain, ServerIP.c_str(), &socketAddress.sin_addr.s_addr);
    if (inetptonResult == 0) {
        struct sockaddr_in* firstGottenAddress = reinterpret_cast<struct sockaddr_in*>(getAddresses(ServerIP, PortString).list->ai_addr);
        socketAddress.sin_addr.s_addr = firstGottenAddress->sin_addr.s_addr;
    } else if (inetptonResult < 0) {
        std::cerr << "test2\n";
        // SHIRANET::ERROR something failed :shrug:
    }
}

void ShiraNet::Sockets::Socket::send(const ShiraNet::NetworkData::Message& Message) {
    const int totalBytesToSend = sizeof(Message.id) + sizeof(Message.payloadSize) + Message.payload.size();

    Logger::debug("Sending " + std::to_string(totalBytesToSend) + " bytes");
    uint32_t networkMessageID = htonl(Message.id);
    uint32_t networkPayloadSize = htonl(Message.payloadSize);
    ssize_t numberOfBytes = ::send(socketID, reinterpret_cast<char*>(&networkMessageID), sizeof(networkMessageID), 0);
    numberOfBytes += ::send(socketID, reinterpret_cast<char*>(&networkPayloadSize), sizeof(networkPayloadSize), 0);
    numberOfBytes += ::send(socketID, Message.payload.data(), Message.payloadSize, 0);

    if (numberOfBytes < 0) {
        Logger::warning("Send failed");
        Exception(ErrorCode::SendFailed, "Failed to send data", errno);
    } else if (numberOfBytes != totalBytesToSend) {
        Logger::warning("Partial send: " + std::to_string(numberOfBytes) + "/" + std::to_string(totalBytesToSend) + " bytes");
        throw Exception(ErrorCode::PartialSend, "Sent partial data");
    }

    Logger::debug("Successfully sent " + std::to_string(numberOfBytes) + " bytes");
}

ShiraNet::NetworkData::Buffer ShiraNet::Sockets::Socket::receive(int AmountOfBytesToRead, int Flags) {
    NetworkData::Buffer receiveBuffer{ AmountOfBytesToRead, "" };
    unsigned int totalBytesReceived = 0;
    receiveBuffer.data.clear();
    receiveBuffer.data.resize(receiveBuffer.size);

    while (totalBytesReceived < AmountOfBytesToRead) {
        ssize_t bytesReceived = 0;
        bytesReceived = ::recv(socketID, &receiveBuffer.data[totalBytesReceived], AmountOfBytesToRead - totalBytesReceived, Flags);
        if (bytesReceived < 0) {
            Logger::warning("Failed to receive data");
            throw Exception(ErrorCode::ReceiveFailed, "Failed to receive data", errno);
        } else if (bytesReceived == 0) {
            Logger::warning("Connection closed early");
            throw Exception(ErrorCode::ConnectionClosedEarly, "Connection closed early");
        }
        totalBytesReceived += bytesReceived;
    }

    return receiveBuffer;
}

ShiraNet::NetworkData::Message ShiraNet::Sockets::Socket::receiveMessage(int Flags) {
    const int messageIDSize = sizeof(uint32_t);
    const int payloadSizeSize = sizeof(uint32_t);
    const int totalBytesToReceive = messageIDSize + payloadSizeSize;

    NetworkData::Buffer receiveBuffer = receive(totalBytesToReceive, Flags);

    uint32_t messageID = 0;
    std::memcpy(&messageID, receiveBuffer.data.data(), sizeof(messageID));
    uint32_t payloadSize = 0;
    std::memcpy(&payloadSize, receiveBuffer.data.data() + messageIDSize, sizeof(payloadSize));

    messageID = ntohl(messageID);
    payloadSize = ntohl(payloadSize);

    ShiraNet::Logger::debug("Message id: " + std::to_string(messageID) + " Payload size:" + std::to_string(payloadSize));

    ShiraNet::NetworkData::Message receivedMessage{ messageID, payloadSize };

    receiveBuffer = receive(payloadSize);
    receivedMessage.payload = receiveBuffer.data;
    return receivedMessage;
}

ShiraNet::Structs::AddressList ShiraNet::Sockets::Socket::getAddresses(const std::string& ServerIP, const std::string& PortString) {
    struct addrinfo addressCriteria{ 0 };
    addressCriteria.ai_family = domain;
    addressCriteria.ai_socktype = type;
    addressCriteria.ai_protocol = protocol;
    ShiraNet::Structs::AddressList returnAddressList{};
    int returnValue = getaddrinfo(ServerIP.c_str(), PortString.c_str(), &addressCriteria, &returnAddressList.list);
    if (returnValue != 0) {
        ShiraNet::Logger::error("getaddrinfo() failed" + std::string(gai_strerror(returnValue)));
    }
    return returnAddressList;
}

std::string ShiraNet::Sockets::Socket::getAddressInfoToStringIP() {
    char addressName[INET_ADDRSTRLEN];
    inet_ntop(domain, &socketAddress.sin_addr.s_addr, addressName, sizeof(addressName));
    return addressName;
}