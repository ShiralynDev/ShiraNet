#pragma once

#include <sys/socket.h>

struct AddressInfo {
    int flags;
    int family;
    int socketType;
    int protocol;
    socklen_t addressLength;
    struct sockaddr *socketAddress;
    char *canonname;
    struct AddressInfo *next;
};