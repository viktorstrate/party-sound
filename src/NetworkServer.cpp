//
// Created by Viktor Hundahl Strate on 30/01/2019.
//

#include "Network.h"

#include "mp3decoder.h"

namespace Network {

    Server::Server() : m_ServerLock(), m_Server(nullptr), m_StopRequested(false) {

    }

    Server::~Server() {
        enet_deinitialize();
    }

    void Server::start(int port) {
        if (enet_initialize() != 0) {
            std::cerr << "An error occurred while initializing ENet for Server." << std::endl;
        }

        ENetAddress address;

        address.host = ENET_HOST_ANY;
        address.port = static_cast<enet_uint16>(port);

        m_Server = enet_host_create(&address /* the address to bind the server host to */,
                                    32      /* allow up to 32 clients and/or outgoing connections */,
                                    2      /* allow up to 2 channels to be used, 0 and 1 */,
                                    0      /* assume any amount of incoming bandwidth */,
                                    0      /* assume any amount of outgoing bandwidth */);

        if (m_Server == nullptr) {
            std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
            exit(EXIT_FAILURE);
        }

        m_ServerThread = std::thread(&Server::eventThread, this);

    }

    void Server::stop() {
        m_StopRequested = true;
        m_ServerThread.join();
    }

    void Server::eventThread() {
        std::cout << "Server background thread" << std::endl;
        ENetEvent event;

        m_ServerLock.lock();

        std::cout << "Server started in another thread: localhost:" << m_Server->address.port << std::endl;

        while (!m_StopRequested) {

            while (enet_host_service(m_Server, &event, 0) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT: {
                        std::cout << "Server: A new client connected from " << event.peer->address.host << ":"
                                  << event.peer->address.port << std::endl;

//                        ENetPacket* packet = enet_packet_create("packet",
//                                                                strlen("packet") + 1,
//                                                                ENET_PACKET_FLAG_RELIABLE);
//
//                        enet_host_broadcast(m_Server, 0, packet);
                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE:
                        std::cout << "Server: Packet received" << std::endl;
                        enet_packet_destroy(event.packet);
                        break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                        std::cout << "Server: Client disconnected" << std::endl;

                    default:
                        break;
                }
            }

            m_ServerLock.unlock();

            usleep(1000*10);
        }

        std::cout << "Stopping server..." << std::endl;
    }

    std::mutex* Server::getServer(ENetHost& server) {
        m_ServerLock.lock();

        server = *m_Server;

        return &m_ServerLock;
    }

};