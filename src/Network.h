//
// Created by Viktor Hundahl Strate on 30/01/2019.
//


#pragma once

#include <enet/enet.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <atomic>

namespace Network {

    class Server {
        ENetHost* m_Server;
        std::mutex m_ServerLock;
        std::thread m_ServerThread;
        std::atomic<bool> m_StopRequested;

    public:
        Server();
        ~Server();
        void start(int port);
        void stop();

    private:
        void eventThread();
    };

    class Client {
        ENetHost* m_Client;
        ENetPeer* m_Peer;
        std::mutex m_ClientLock;
        std::thread m_ClientThread;
        std::atomic<bool> m_StopRequested;


    public:
        Client();

        virtual ~Client();

    public:
        void connect(const ENetAddress &host);
        void stop();

    private:
        void eventThread();
    };
}

