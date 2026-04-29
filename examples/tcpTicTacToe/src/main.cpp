#include <ShiraNet.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>

bool isPlayerX = false;
bool xTurn = true;

struct placedMarker {
    int xPos;
    int yPos;
    bool playerX;
};

std::vector<placedMarker> markers;

void clearAndDrawBoard() {
#ifdef __unix__
    system("clear");
#else
    system("cls");
#endif

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            bool placedMarker = false;
            for (auto marker : markers) {
                if (marker.xPos == j && marker.yPos == i) {
                    if (isPlayerX)
                        std::cout << "x";
                    else
                        std::cout << "o";
                    placedMarker = true;
                    break;
                }
            }
            if (!placedMarker)
                std::cout << "-";
        }
        std::cout << std::endl;
    }
}

void playTurn() {
    clearAndDrawBoard();
    if (xTurn && isPlayerX || !xTurn && !isPlayerX) {
        std::cout << "Enter x & y pos, (xy)\n";
        std::string input;
        std::cin >> input;
        int xPos = std::stoi(input.substr(0, 1));
        int yPos = std::stoi(input.substr(1, 1));
        markers.push_back({ xPos, yPos, isPlayerX });
        clearAndDrawBoard();
    } else {
    }
}

void handleClient(std::shared_ptr<ShiraNet::Sockets::TcpSocket> client, ShiraNet::Servers::TcpServer* server) {
    std::cout << "Some client connected: " << client->getAddressInfoToStringIP() << std::endl;
}

void host() {
    ShiraNet::Servers::TcpServer server(AF_INET, 1337, 1);

    server.getConnection(handleClient);
    isPlayerX = rand() % 1;

    std::cout << isPlayerX << std::endl;

    ShiraNet::NetworkData::Message playerTypeMessage(0);
    ShiraNet::NetworkData::DataField<bool> playerTypeData;
    playerTypeData.data = !isPlayerX;
    playerTypeData.size = sizeof(bool);
    playerTypeMessage.dataFieldToPayload(playerTypeData);
    server.sendToAll(playerTypeMessage);

    while (true) {
        playTurn();
        ShiraNet::NetworkData::Message markerPlacementMessage(1);
        ShiraNet::NetworkData::DataField<std::vector<placedMarker>> markerPlacementData;
        markerPlacementData.data = markers;
        markerPlacementData.size = sizeof(markerPlacementData.data);
        markerPlacementMessage.dataFieldToPayload(markerPlacementData);
        server.sendToAll(markerPlacementMessage);

        bool gotMessage = false;
        while (!gotMessage) {
            try {
                ShiraNet::NetworkData::Message message{ server.getClient(0)->receiveMessage() };
                gotMessage == true;
            } catch (...) {
            }
        }
    }
}

void client() {
    std::cout << "Please enter the ip for the server\n";
    std::string inputIp;
    std::cin >> inputIp;
    ShiraNet::Sockets::TcpSocket client(AF_INET);
    client.connect(inputIp, 1337);

    while (true) {
        try {
            ShiraNet::NetworkData::Message message{ client.receiveMessage() };

            std::istringstream stream(message.payload, std::ios::binary);

            switch (message.id) {
                case 0:
                    bool value;
                    stream.read(reinterpret_cast<char*>(&value), sizeof(value));
                    std::cout << value;
                    break;

                default:
                    break;
            }
        } catch (...) {
        };
    }
}

int main() {
    int i = 0;
    std::cout << "Act as host [1], Act as client [2]\n";
    std::cin >> i;

    if (i == 1) {
        std::cout << "Acting as host\n";
        host();
    } else if (i == 2) {
        std::cout << "Acting as client\n";
        client();
    } else {
        return 0;
    }

    return 0;
}
