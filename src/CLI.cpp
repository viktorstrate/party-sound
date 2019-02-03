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
        std::cout << "Please select the host to connect to:" << std::endl;

        std::string host;
        std::cin >> host;


        std::size_t separator = host.find(':');

        ENetAddress serverAddr;

        if (separator == std::string::npos) {
            enet_address_set_host(&serverAddr, host.c_str());
        } else {

            int port = stoi(host.substr(separator+1));
            host = host.substr(0, separator);

            enet_address_set_host(&serverAddr, host.c_str());
            serverAddr.port = static_cast<enet_uint16>(port);
        }

        auto client = Network::Client();
        client.connect(serverAddr);

        std::cout << "Press enter to stop client" << std::endl;

        {
            int empty;
            std::cin >> empty;
        }

        client.stop();
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

