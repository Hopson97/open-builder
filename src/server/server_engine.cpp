#include "server_engine.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <common/network/net_constants.h>
#include <iostream>
#include <thread>

const int MAX_CONNECTS = 10;

ServerEngine::ServerEngine()
    : m_world(MAX_CONNECTS)
    , m_host({ENET_HOST_ANY, DEFAULT_PORT}, MAX_CONNECTS)
    , m_clients(MAX_CONNECTS)
    , m_maxConnections(MAX_CONNECTS)
    , m_salt(createHandshakeRandom())
{
    m_clientsMap.reserve(MAX_CONNECTS);
}

ServerEngine::~ServerEngine()
{
    stop();
}

bool ServerEngine::isSetup() const
{
    return m_host.handle != nullptr;
}

void ServerEngine::run()
{
    printf("Type 'exit' to shutdown server");
    m_serverThread = std::thread([this] {
        std::string input;
        while (m_isServerRunning) {
            std::getline(std::cin, input);
            if (input == "exit") {
                std::cout << "Exiting server.\n\n";
                m_isServerRunning = false;
            }
        }
    });

    launch();
}

void ServerEngine::runAsThread()
{
    m_serverThread = std::thread([this] { launch(); });
}

void ServerEngine::launch()
{
    if (!isSetup()) {
        return;
    }

    m_isServerRunning = true;

    while (m_isServerRunning) {

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        tick();

        m_world.tick();

        broadcastEntityStates();
    }
}

void ServerEngine::tick()
{
    assert(m_host.handle);
    NetEvent event;
    while (m_host.pumpEvent(event)) {
        switch (event.type) {
            case NetEventType::Connection:
                addPendingConnection(event.peer);
                break;

            case NetEventType::Disconnect:
            case NetEventType::Timeout:
                handleDisconnection(event.peer);
                break;

            case NetEventType::Data: {
                ServerPacket packet(event.packet);
                handlePacket(packet, event.peer);
                enet_packet_destroy(event.packet);
            } break;

            default:
                break;
        }
    }

    for (auto& client : m_clients) {
        if (client.isActive()) {
            client.tick(m_world);
        }
    }
}

void ServerEngine::stop()
{
    m_isServerRunning = false;
    if (m_isServerRunning) {

        broadcastServerShutdown();

        enet_host_flush(m_host.handle);
        for (auto& session : m_clients) {
            session.disconnect();
        }
    }
    if (m_serverThread.joinable()) {
        m_serverThread.join();
    }
}

void ServerEngine::broadcastPacket(ServerPacket& packet, int channel, int flags)
{
    for (auto& client : m_clients) {
        if (client.isActive())
            client.sendPacket(packet, channel, flags);
    }
}

std::vector<PendingClientSession>::iterator ServerEngine::findPendingSession(u32 peerId)
{
    for (auto itr = m_pendingConnections.begin(); itr != m_pendingConnections.end();) {
        auto thepeer = itr->connection.peer;
        if (thepeer->incomingPeerID == peerId) {
            return itr;
        }
    }
    return m_pendingConnections.end();
}

void ServerEngine::addPendingConnection(ENetPeer* peer)
{
    PendingClientSession session;
    session.connection.peer = peer;
    m_pendingConnections.push_back(session);
}

int ServerEngine::createClientSession(ENetPeer* peer, u32 salt)
{
    for (unsigned i = 0; i < m_clients.size(); i++) {
        if (!m_clients[i].isActive()) {
            u32 playerId = m_world.addEntity();
            m_clients[i].init(peer, salt, playerId);
            m_clientsMap[peer->incomingPeerID] = i;
            broadcastPlayerJoin(playerId);
            return i;
        }
    }
    return -1;
}

//
// B R O A D C A S T S
//
void ServerEngine::broadcastEntityStates()
{
    ServerPacket packet(ClientCommand::UpdateEntityStates, m_salt);
    m_world.serialiseEntities(packet);

    broadcastPacket(packet);
}

void ServerEngine::broadcastPlayerJoin(u32 playerId)
{
    ServerPacket packet(ClientCommand::AddEntity, m_salt);
    packet.write((u32)1);
    packet.write(playerId);
    std::cout << "Player join " << playerId << std::endl;
    auto& player = m_world.findEntity(playerId);
    packet.write(player.position);
    packet.write(player.rotation);

    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ServerEngine::broadcastPlayerLeave(u32 playerId)
{
    ServerPacket packet(ClientCommand::RemoveEntity, m_salt);
    packet.write(playerId);
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}

void ServerEngine::broadcastServerShutdown()
{
    ServerPacket packet(ClientCommand::ForceExitGame, m_salt);
    packet.write(std::string("Host has exited the game."));
    broadcastToPeers(m_host.handle, packet.get(), 0, ENET_PACKET_FLAG_RELIABLE);
}