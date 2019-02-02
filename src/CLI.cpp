//
// Created by Viktor Hundahl Strate on 01/02/2019.
//

#include "CLI.h"

#include <iostream>
#include "Network.h"

namespace CLI {

    void Menu::showMenu() {
        int choise;

        do {
            std::cout << "=== " << menuTitle << " ===" << std::endl;

            for (int i = 0; i < items.size(); i++) {
                auto item = items[i];

                std::cout << i+1 << ") " << item.title << std::endl;
            }

            std::cout << "0) Return" << std::endl;


            std::cin >> choise;

            if (choise <= 0 || choise > items.size()) {
                continue;
            }

            items[choise-1].onSelect();

        } while(choise != 0);
    }

    void startServer() {
        std::cout << "Please select a port to listen on:" << std::endl;

        int port;
        std::cin >> port;


        auto server = Network::Server();
        server.start(port);

        std::cout << "Press enter to stop server" << std::endl;

        {
            int empty;
            std::cin >> empty;
        }

        server.stop();

    }

    void startClient() {
        std::cout << "Please select the port to connect to:" << std::endl;

        int port;
        std::cin >> port;

        ENetAddress serverAddr;

        serverAddr.host = ENET_HOST_ANY;
        serverAddr.port = static_cast<enet_uint16>(port);

        auto client = Network::Client();
        client.connect(serverAddr);
    }

    void start() {

        MenuItem server;
        server.title = "Start server";
        server.onSelect = startServer;

        MenuItem client;
        client.title = "Connect to server";
        client.onSelect = startClient;

        Menu mainMenu;

        mainMenu.menuTitle = "Main Menu";
        mainMenu.items.push_back(server);
        mainMenu.items.push_back(client);

        mainMenu.showMenu();
    }

}

