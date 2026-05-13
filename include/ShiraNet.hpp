#pragma once

#ifdef __linux__
#include <sys/socket.h>
#elif _WIN32
#include <winsock2.h>
#else
#endif

#include "../src/buffer/buffer.hpp"
#include "../src/server/tcp/tcpServer.hpp"
#include "../src/sockets/tcp/tcpSocket.hpp"