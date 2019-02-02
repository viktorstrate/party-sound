//
// Created by Viktor Hundahl Strate on 31/01/2019.
//

#include "Network.h"

namespace Network {

    Client::Client() = default;

    Client::~Client() {
        enet_host_destroy(m_Client);
    }

    void Client::connect(const ENetAddress &host) {

        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet for Client." << std::endl;
        }

        m_Client = enet_host_create (nullptr /* create a client host */,
                                   1 /* only allow 1 outgoing connection */,
                                   2 /* allow up 2 channels to be used, 0 and 1 */,
                                   0 /* assume any amount of incoming bandwidth */,
                                   0 /* assume any amount of outgoing bandwidth */);

        if (m_Client == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
            exit (EXIT_FAILURE);
        }

        m_Peer = enet_host_connect(m_Client, &host, 2, 0);
    }

}