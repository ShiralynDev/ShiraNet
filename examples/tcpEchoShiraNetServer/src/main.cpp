#include <ShiraNet.hpp>

#include <iostream>
#include <cstring>

#define BUFFERSIZE 1024

void server() {
    ShiraNet::Servers::TcpServer server(AF_INET, 57942, 5); // this is not the best example for servers since this handles one client at a time
    while (true) { 
        int newClientIndex = server.getConnection();
        std::cout << server.clientSockets.at(newClientIndex).getAddressInfoToStringIP() << std::endl;
        Buffer bufferToSend = server.clientSockets.at(newClientIndex).receive(BUFFERSIZE);
        bufferToSend.size = BUFFERSIZE;
        server.clientSockets.at(newClientIndex).send(bufferToSend);
    }
}

void client() {
    ShiraNet::Sockets::TcpSocket socket(AF_INET);
    socket.connect("localhost", 57942);

    std::cout << "Enter data to send\n";
    std::string dataToSend;
    std::cin.ignore();
    std::getline(std::cin, dataToSend);
    dataToSend += "\n";

    Buffer bufferToSend{BUFFERSIZE, dataToSend};
    socket.send(bufferToSend);
    Buffer data = socket.receive(BUFFERSIZE);
    std::cout << data.data;
}

int main() {
    int i = 0;
    std::cout << "Act as server [1], Act as client [2]\n";
    std::cin >> i;
    
    if (i == 1) {
        std::cout << "Acting as server\n";
        server();
    } else if (i == 2) {
        std::cout << "Acting as client\n";
        client();
    } else {
        return 0;
    }
    
    return 0;
}
