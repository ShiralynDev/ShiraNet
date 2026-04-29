#pragma once

#include "../../buffer/buffer.hpp"
#include "../../host/address/address.hpp"
#include "../../utils/utils.hpp"

#include <netinet/in.h>

#include <string>

namespace ShiraNet::Sockets {

    /**
     * @brief Represents a socket.
     *
     * Can be used directly but it's preferred to use the premade expansions like ShiraNet::Sockets::TcpSocket
     *
     * - Has the socket file descriptor
     * - Opens socket on construction
     * - Closes on destruction
     */
    class Socket : public Utils::NoCopy {
    private:
    protected:
        int socketID = 0;
        int domain = 0;
        int type = 0;
        int protocol = 0;
        bool isValid = 0;
        sockaddr_in socketAddress{ 0 };

        void addStringIPToAddressInfo(const std::string& ServerIP, const std::string& PortString);

    public:
        Socket();
        Socket(int Domain, int Type, int Protocol);
        Socket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress);
        Socket(Socket&& other) noexcept;
        Socket& operator=(Socket&& other) noexcept;
        ~Socket();

        bool isSocketValid() { return isValid; };
        void send(const NetworkData::Message& Message);
        NetworkData::Buffer receive(int bytesToRead);
        NetworkData::Message receiveMessage();
        ShiraNet::Structs::AddressList getAddresses(const std::string& ServerIP, const std::string& PortString);
        std::string getAddressInfoToStringIP();
    };

} // namespace ShiraNet::Sockets