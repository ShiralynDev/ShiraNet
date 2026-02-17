#pragma once

#include "../../utils/utils.hpp"
#include "../../buffer/buffer.hpp"
#include "../../host/address/address.hpp"

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
            sockaddr_in socketAddress{0};

            void addStringIPToAddressInfo(char* ServerIP, std::string PortString);

        public:
            Socket(int Domain, int Type, int Protocol);
            Socket(int SocketID, int Domain, int Type, int Protocol, sockaddr_in SocketAddress);
            Socket(Socket&& other) noexcept;
            Socket& operator=(Socket&& other) noexcept;
            ~Socket();
            
            void send(Buffer& buffer);
            Buffer receive(unsigned int AmountOfBytesToRead);
            ShiraNet::Structs::AddressList getAddresses(char* ServerIP, std::string PortString);
            std::string getAddressInfoToStringIP();
    };

}