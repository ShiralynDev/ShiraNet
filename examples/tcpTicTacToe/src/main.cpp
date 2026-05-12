/*
This is a bad example, don't code like this
*/

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

    void serialize(std::ostream& out) const {
        out << xPos << ' ';
        out << yPos << ' ';
        out << playerX << ' ';
    }

    void deserialize(std::istream& in) {
        in >> xPos >> yPos >> playerX;
    }
};

int winConditions(std::vector<placedMarker> markers) {
    return 0;
}

void clearAndDrawBoard(std::vector<placedMarker> markers) {
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
                    if (marker.playerX)
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

placedMarker playTurn() {
    std::cout << "Enter x & y pos, (xy)\n";
    std::string input;
    std::cin >> input;
    int xPos = std::stoi(input.substr(0, 1));
    int yPos = std::stoi(input.substr(1, 1));
    return placedMarker{ xPos - 1, yPos - 1, isPlayerX };
}

void handleClient(std::shared_ptr<ShiraNet::Sockets::TcpSocket> client, ShiraNet::Servers::TcpServer* server) {
    std::cout << "Some client connected: " << client->getAddressInfoToStringIP() << std::endl;
}

void host() {
    ShiraNet::Servers::TcpServer server(AF_INET, 1337, 1);

    server.getConnection(handleClient);

    isPlayerX = rand() % 2;
    std::vector<placedMarker> markers;

    while (true) {
        clearAndDrawBoard(markers);
        if (xTurn && isPlayerX || !xTurn && !isPlayerX) {
            markers.push_back(playTurn());
            ShiraNet::NetworkData::Message markerPlacementMessage(2, markers);
            server.sendToAll(markerPlacementMessage);
            xTurn = !xTurn;
        } else {
            server.sendToAll(1);
            ShiraNet::NetworkData::Message otherClientsPlacement = server.getClient(0)->receiveMessage();
            ShiraNet::NetworkData::DataField<placedMarker> placedMarker;
            otherClientsPlacement.payloadToDataField(placedMarker);
            markers.push_back({ placedMarker.data.xPos, placedMarker.data.yPos, !isPlayerX });
            xTurn = !xTurn;
        }
        clearAndDrawBoard(markers);
        if (winConditions(markers)) {
            return;
        }
    }
}

void client() {
    std::cout << "Please enter the ip for the server\n";
    std::string inputIp;
    std::cin >> inputIp;
    ShiraNet::Sockets::TcpSocket client(AF_INET);
    client.connect(inputIp, 1337);

    std::vector<placedMarker> markers;

    clearAndDrawBoard(markers);

    while (true) {
        try {
            ShiraNet::NetworkData::Message message{ client.receiveMessage() };

            switch (message.id) {
                case 1: {
                    if (markers.size() == 0)
                        isPlayerX = true;

                    placedMarker markerPlacement;
                    markerPlacement = playTurn();
                    markers.push_back(markerPlacement);

                    ShiraNet::NetworkData::Message markerPlacementMessage(2, markerPlacement);
                    client.send(markerPlacementMessage);

                    clearAndDrawBoard(markers);
                    break;
                }

                case 2: {
                    ShiraNet::NetworkData::DataField<std::vector<placedMarker>> data;
                    message.payloadToDataField(data);
                    markers = data.data;
                    clearAndDrawBoard(markers);
                    break;
                }

                default:
                    break;
            }
        } catch (...) {
            std::cout << "Host closed session\n";
            return;
        };
        if (winConditions(markers)) {
            return;
        }
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
