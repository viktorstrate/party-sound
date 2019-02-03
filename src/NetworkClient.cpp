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

        m_Client = enet_host_create(nullptr /* create a client host */,
                                    1 /* only allow 1 outgoing connection */,
                                    2 /* allow up 2 channels to be used, 0 and 1 */,
                                    0 /* assume any amount of incoming bandwidth */,
                                    0 /* assume any amount of outgoing bandwidth */);

        if (m_Client == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet client host." << std::endl;
            exit(EXIT_FAILURE);
        }

        m_Peer = enet_host_connect(m_Client, &host, 2, 0);

        if (m_Peer == nullptr) {
            std::cerr << "Could not connect to peer" << std::endl;
            return;
        }

        m_ClientThread = std::thread(&Client::eventThread, this);
    }

    void Client::stop() {
        m_StopRequested = true;
        m_ClientThread.join();
    }

    void Client::eventThread() {
        std::cout << "Client background thread" << std::endl;
        ENetEvent event;

        m_ClientLock.lock();

        std::cout << "Client set up to: localhost:" << m_Peer->address.port << std::endl;

        if (enet_host_service(m_Client, &event, 5000) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << "Connection succeeded." << std::endl;
        } else {
            enet_peer_reset(m_Peer);
            std::cerr << "Connection failed." << std::endl;
            return;
        }

        while (!m_StopRequested) {

            while (enet_host_service(m_Client, &event, 0) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT:

                        std::cout << "Client: A new client connected from " << event.peer->address.host << ":"
                                  << event.peer->address.port << std::endl;
                        break;

                    case ENET_EVENT_TYPE_RECEIVE:
                        std::cout << "Client: Packet received" << std::endl;
                        enet_packet_destroy(event.packet);
                        break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                        std::cout << "Client: Client disconnected" << std::endl;

                    default:
                        break;
                }
            }

            m_ClientLock.unlock();

            usleep(1000*10);
        }

        std::cout << "Stopping client..." << std::endl;
    }

}