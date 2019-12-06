#include "server_engine.h"

#include "network/server.h"
#include "server_config.h"
#include <SFML/System/Clock.hpp>
#include <array>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/packet.h>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace {
} // namespace

void runServerEngine(const ServerConfig &config, sf::Time timeout)
{
    // Start the main server loop
    sf::Clock deltaClock;
    sf::Clock timeoutClock;

    Server server;
    server.socket.setBlocking(false);
    server.socket.bind(DEFAULT_PORT);

    bool isRunning = true;
    while (isRunning) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        auto deltaTime = deltaClock.restart();

		//=========================
		// Handle incoming packets
		//
        Packet incoming;
        Endpoint endpoint;
        while (receivePacket(server.socket, incoming, endpoint)) {
            switch (static_cast<ServerCommand>(incoming.command)) {

                case ServerCommand::Connect: {
                    LOG("Server: Connection request got")
                    int slot = server.clients.addClient(endpoint);
                    if (slot >= 0) {
                        auto packet =
                            makePacket(ClientCommand::AcceptConnection);
                        packet.data << static_cast<client_id_t>(slot);
                        server.sendPacket(slot, packet);

                        auto broadcast =
                            makePacket(ClientCommand::PlayerJoin);
                        broadcast.data << static_cast<client_id_t>(slot);
                        server.broadcastPacket(broadcast);

                        // TODO Tell the new player the world/entity state
                    }
                    else {
                        auto packet =
                            makePacket(ClientCommand::RejectConnection);
                        server.socket.send(packet.data, endpoint.address,
                                           endpoint.port);
                    }
                } break;

                case ServerCommand::ConnectionChallengeResponse:
                    break;

                case ServerCommand::Disconnect: {
                    client_id_t id = 0;
                    incoming.data >> id;
                    server.clients.removeClient(id);

					auto broadcast = makePacket(ClientCommand::PlayerJoin);
                    broadcast.data << static_cast<client_id_t>(id);
					server.broadcastPacket(broadcast);
                } break;
            }
        }


		//==============
		//	Check if server should auto shutdown
		//
        if (server.clients.connectedCount() == 0) {
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
