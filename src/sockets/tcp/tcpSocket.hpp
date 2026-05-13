#pragma once

#include "../socket/socket.hpp"

#ifdef __linux__
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#endif

/**
 * @file TcpSocket.hpp
 * @brief TCP socket abstraction for ShiraNet.
 * Contains the TcpSocket class in the Sockets namespace
 */

namespace ShiraNet::Sockets {

    /**
     * @brief Represents a TCP socket by expanding the socket class.
     *
     * - Opens socket on construction with predefined args
     */
    class TcpSocket : public Socket {
    private:
    public:
        TcpSocket();
        TcpSocket(int Domain);
        TcpSocket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress);
        void connect(const std::string& ServerIP, const in_port_t& ServerPort);
        void bind(const in_port_t& ServerPort, const uint32_t& ServerIP = INADDR_ANY);
        void listen(int maxPendingRequest);
        TcpSocket getClientConnection();
    };

} // namespace ShiraNet::Sockets