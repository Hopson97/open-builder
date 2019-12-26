#pragma once

#include <atomic>
#include <catch2/catch.hpp>
#include <common/network/net_host.h>
#include <thread>

class TestServer : public NetworkHost {
  public:
    TestServer()
        : NetworkHost("Server")
    {
    }

  private:
    void onPeerConnect(ENetPeer *peer) override
    {
        sf::Packet packet;
        packet << ClientCommand::PeerId << static_cast<peer_id_t>(0);

        sendToPeer(peer, packet, 0, ENET_PACKET_FLAG_RELIABLE);
    }

    void onPeerDisconnect(ENetPeer *peer) override
    {
    }

    void onPeerTimeout(ENetPeer *peer) override
    {
    }

    void onCommandRecieve(ENetPeer* peer, sf::Packet &packet, command_t command) override
    {
    }
};

class TestClient : public NetworkHost {
  public:
    TestClient()
        : NetworkHost("Client")
    {
    }

  private:
    void onPeerConnect(ENetPeer *peer) override
    {
    }

    void onPeerDisconnect(ENetPeer *peer) override
    {
    }

    void onPeerTimeout(ENetPeer *peer) override
    {
    }

    void onCommandRecieve(ENetPeer* peer, sf::Packet &packet, command_t command) override
    {
    }
};


TEST_CASE("The server works")
{
    TestServer server;
    SECTION("The server is able to be created")
    {
        REQUIRE(server.createAsServer(4) == true);
        REQUIRE(server.getMaxConnections() == 4);
        REQUIRE(server.getConnectedPeerCount() == 0);
    }
}

TEST_CASE("The client can interact with the server.")
{
    TestServer server;
    server.createAsServer(4);

    SECTION("The client is able to connect to the server")
    {
        std::atomic<bool> control = true;
        std::thread serverThread([&control, &server]() {
            while (control) {
                server.tick();
            }
        });

        TestClient client;
        auto serverConnection = client.createAsClient(LOCAL_HOST);

        REQUIRE(serverConnection.has_value() == true);
        REQUIRE(server.getConnectedPeerCount() == 1);
        REQUIRE(client.getConnectedPeerCount() == 1);

        control = false;

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

        TestClient client;
        auto serverConnection = client.createAsClient(LOCAL_HOST);
        client.disconnectFromPeer(*serverConnection);

        REQUIRE(server.getConnectedPeerCount() == 0);
        REQUIRE(client.getConnectedPeerCount() == 0);

        control = false;

        serverThread.join();
    }
}