#include "server_engine.h"

#include "server_config.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <common/debug.h>
#include <SFML/System/Clock.hpp>
#include <array>
#include <common/network/net_types.h>
#include <iostream>
#include <thread>

namespace {
struct ClientManager {
    std::array<Endpoint, MAX_CONNECTIONS> endpoints;
    std::array<bool, MAX_CONNECTIONS> isConnected;
    int currentConnections = 0;
};
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    // Start the main server loop
    sf::Clock deltaClock;
    sf::Clock timeoutClock;
    sf::UdpSocket socket;
    ClientManager clients;
    bool isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        auto deltaTime = deltaClock.restart();

        sf::Packet packet;
        sf::IpAddress remoteAddress;
        port_t remotePort;
        if (socket.receive(packet, remoteAddress, remotePort) ==
            sf::Socket::Done) {
        }

        if (clients.currentConnections == 0) {
            if (timeoutClock.getElapsedTime() >= timeout) {
                isRunning = false;
			}
        }
        else {
            timeoutClock.restart();
        }
        (void)deltaTime;
        (void)config;
    }
    LOG("Server exiting");
}
