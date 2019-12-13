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
        std::atomic<bool> run = true;
        std::thread serverThread([&run, &server]() {
            while (run) {
                server.tick();
            }
        });

        Client client;
        auto serverConnection = client.connectToServer(LOCAL_HOST);
        if (serverConnection) {
            std::cout << "TRUUUUUUUUUUUU\n";
        }
        else {
            std::cout << "falseeeeeeeeee\n";
        }

        REQUIRE(serverConnection.has_value() == true);

        run = false;

        server.destroy();
        client.destroy();

        serverThread.join();
    }
}