#pragma once

#include <client/network/client.h>
#include <server/network/server.h>

#include <catch2/catch.hpp>
#include <future>
#include <thread>

TEST_CASE("Client is able to connect to the server")
{
    Server server;

    auto f = std::async(std::launch::async, []() {
        ClientConnection client;
        return client.connectTo(sf::IpAddress::LocalHost);
    });

    for (int i = 0; i < 10000; i++) {
        Packet packet;
        while (receivePacket(server.socket, packet)) {
            if (static_cast<ServerCommand>(packet.command) == ServerCommand::Connect) {
                server.tryConnectClient(packet);
            }
        }
    }

    auto result = f.get();

    REQUIRE(result == true);
}