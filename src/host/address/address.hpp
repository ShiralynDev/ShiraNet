#pragma once

#include "../../utils/utils.hpp"

#include <cstdint>

#ifdef __linux__
#include <netdb.h>
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#endif

#ifndef in_port_t
typedef uint16_t in_port_t;
#endif

#ifdef _WIN32
typedef uint32_t in_addr_t;
#endif

namespace ShiraNet::Structs {

    struct AddressList : public ShiraNet::Utils::NoCopy {
        struct addrinfo* list{ 0 };

        AddressList() = default;
        ~AddressList() {
            if (list)
                freeaddrinfo(list);
        }

        AddressList(AddressList&& other) noexcept
          : list(other.list) {
            other.list = nullptr;
        }

        AddressList& operator=(AddressList&& other) noexcept {
            if (this != &other) {
                if (list)
                    freeaddrinfo(list);
                list = other.list;
                other.list = nullptr;
            }
            return *this;
        }
    };

}