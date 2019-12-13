#pragma once

#include <common/network/net_host.h>
#include <catch2/catch.hpp>
#include <atomic>
#include <thread>

class Server : public NetworkHost
{
    public:
        Server()
        :   NetworkHost("Server")
        {}
    
    private:
        void onPeerConnect(ENetPeer &peer) override
        {

        }

        void onPeerDisconnect(ENetPeer &peer) override
        {

        }

        void onPeerTimeout(ENetPeer &peer) override
        {

        }

        void onCommandRecieve(sf::Packet &packet, command_t command) override
        {

        }
};

class Client : public NetworkHost
{    
    public:
        Client()
        :   NetworkHost("Client")
        {}

    private:
        void onPeerConnect(ENetPeer &peer) override
        {

        }

        void onPeerDisconnect(ENetPeer &peer) override
        {

        }

        void onPeerTimeout(ENetPeer &peer) override
        {

        }

        void onCommandRecieve(sf::Packet &packet, command_t command) override
        {

        }
};

TEST_CASE("The server works") 
{
    Server server;
    SECTION( "The server is able to be created" ) {
        REQUIRE(server.createAsServer() == true);
    }
}

TEST_CASE("The client can interact with the server.") 
{
    Server server;
    server.createAsServer();

    SECTION( "The client is able to connect to the server" ) {
        std::atomic<bool> run = true;
        std::thread serverThread([&run, &server](){
            while (run) 
            {
                server.tick();
            }
        });
        
        Client client;
        REQUIRE(client.connectToServer(LOCAL_HOST) != nullptr);

        run = false;
        serverThread.join();
    }
}