#pragma once

#include "../../utils/utils.hpp"

#include <netdb.h>
#include <sys/socket.h>

namespace ShiraNet::Structs {

    struct AddressInfo { // not used rn
        int flags;
        int family;
        int socketType;
        int protocol;
        socklen_t addressLength;
        struct sockaddr* socketAddress;
        char* canonname;
        struct AddressInfo* next;
    };

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