#include <ShiraNet.hpp>

#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

void handleClient(std::shared_ptr<ShiraNet::Sockets::TcpSocket> Client, ShiraNet::Servers::TcpServer *Server) {
    try {
        while (true) {
            ShiraNet::NetworkData::Message message = Client->receiveMessage();
            Server->sendToAllExcept(message, Client);
        }
    } catch (...) {
        std::cout << Client->getAddressInfoToStringIP() + " disconnected\n";
        Server->removeClient(Client);
    }
}

void server() {
    ShiraNet::Servers::TcpServer server(AF_INET, 57942, 5);

    while (true) {
        server.getConnection(handleClient);
    }
}

void sendMessage(ShiraNet::Sockets::TcpSocket *Socket, std::string *Name) {
    while (true) {
        std::string text;
        std::getline(std::cin, text);

        if (text == "!q") {
            ShiraNet::NetworkData::Message messageToSend(2, *Name);
            Socket->send(messageToSend);
            delete Socket;
            return;
        }

        ShiraNet::NetworkData::Message messageToSend(1, text);
        Socket->send(messageToSend);
    }
}

void getMessage(ShiraNet::Sockets::TcpSocket *Socket) {
    while (true) {
        ShiraNet::NetworkData::Message message{Socket->receiveMessage()};

        switch (message.id) {
        case 0:
            std::cout << message.payload + " connected\n";
            break;

        case 1:
            std::cout << message.payload + "\n";
            break;

        case 2:
            std::cout << message.payload + " disconnected\n";
            break;

        default:
            break;
        }
    }
}

void client() {
    ShiraNet::Sockets::TcpSocket socket(AF_INET);
    socket.connect("localhost", 57942);

    std::cout << "Enter name:\n";
    std::string name;
    std::cin.ignore();
    std::getline(std::cin, name);

    ShiraNet::NetworkData::Message messageToSend(0, name);
    socket.send(messageToSend);

    std::thread sendMessageThread{sendMessage, &socket, &name};
    std::thread{getMessage, &socket}.detach();

    while (true) {
        if (sendMessageThread.joinable()) {
            sendMessageThread.join();
            return;
        }
    }
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
