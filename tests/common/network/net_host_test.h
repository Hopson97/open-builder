#pragma once

#include <atomic>
#include <catch2/catch.hpp>
#include <common/network/net_host.h>
#include <thread>

class Server : public NetworkHost {
  public:
    Server()
        : NetworkHost("Server")
    {
    }

  private:
    void onPeerConnect(ENetPeer &peer) override
    {
        sf::Packet packet;
        packet << ClientCommand::ClientId << static_cast<peer_id_t>(0);

        sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);
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

class Client : public NetworkHost {
  public:
    Client()
        : NetworkHost("Client")
    {
    }

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
    SECTION("The server is able to be created")
    {
        REQUIRE(server.createAsServer() == true);
    }
    server.destroy();
}

TEST_CASE("The client can interact with the server.")
{
    Server server;
    server.createAsServer();

    SECTION("The client is able to connect to the server")
    {
        std::atomic<bool> control = true;
        std::thread serverThread([&control, &server]() {
            while (control) {
                server.tick();
            }
        });

        Client client;
        auto serverConnection = client.connectToServer(LOCAL_HOST);

        REQUIRE(serverConnection.has_value() == true);
        REQUIRE(server.getConnectedPeerCount() == 1);
        REQUIRE(client.getConnectedPeerCount() == 1);

        control = false;
        client.destroy();

        serverThread.join();
    }
    
    SECTION("The client can disconnect from the server")
    {
        std::atomic<bool> control = true;
        std::thread serverThread([&control, &server]() {
            while (control) {
                server.tick();
            }
        });

        Client client;
        auto serverConnection = client.connectToServer(LOCAL_HOST);
        client.disconnectFromPeer(**serverConnection);

        REQUIRE(server.getConnectedPeerCount() == 0);
        REQUIRE(client.getConnectedPeerCount() == 0);

        control = false;
        client.destroy();

        serverThread.join();
    }
    

    server.destroy();
}