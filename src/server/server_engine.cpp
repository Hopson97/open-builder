#include "server_engine.h"
#include <SFML/System/Clock.hpp>
#include <atomic>
#include <common/debug.h>
#include <common/network/net_constants.h>
#include <iostream>
#include <thread>

const int MAX_CONNECTS = 10;

ServerEngine::ServerEngine()
    : m_world(16)
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

////////////////////////////////////////////////////////
//
//          Packet handling functions
//
#define AUTHENTICATE_PACKET                                                              \
    auto itr = m_clientsMap.find(peer->incomingPeerID);                                  \
    if (itr == m_clientsMap.end()) {                                                     \
        return;                                                                          \
    }                                                                                    \
    auto& client = m_clients.at(itr->second);                                            \
    if (!client.verify(packet.getSalt())) {                                              \
        return;                                                                          \
    }

void ServerEngine::handlePacket(ServerPacket& packet, ENetPeer* peer)
{
    using Cmd = ServerCommand;
    // clang-format off
    switch (packet.command()) {
        case Cmd::HandshakePartOne:     onHandshakePartOne(packet, peer);   break;
        case Cmd::HandshakeResponse:    onHandshakeResponse(packet, peer);  break;

        case Cmd::Interaction:  onInteraction(packet, peer);    break;
        case Cmd::MouseState:   onMouseState(packet, peer);     break;
        case Cmd::PlayerState:  onPlayerState(packet, peer);    break;
        case Cmd::SpawnRequest: onSpawnRequest(packet, peer);   break;
    }
    // clang-format on
}

void ServerEngine::onHandshakePartOne(ServerPacket& packet, ENetPeer* peer)
{
    for (auto& pending : m_pendingConnections) {
        if (pending.connection.peer->incomingPeerID == peer->incomingPeerID) {
            pending.salt = packet.getSalt();
            pending.sendHandshakeChallenge(m_salt);
        }
    }
}

void ServerEngine::onHandshakeResponse(ServerPacket& packet, ENetPeer* peer)
{
    auto itr = findPendingSession(peer->incomingPeerID);
    if (itr != m_pendingConnections.end()) {
        auto& pending = *itr;
        auto thepeer = pending.connection.peer;
        if (thepeer->incomingPeerID == peer->incomingPeerID) {
            u32 salt = itr->salt ^ m_salt;
            if (salt == packet.getSalt()) {
                itr->salt = salt;
                int slot = createClientSession(peer, salt);
                if (slot != -1) {
                    pending.sendAcceptConnection(m_clients[slot].getPlayerId());
                    pending.sendGameData(m_world);
                }
                else {
                    pending.sendRejectConnection("Game Full");
                }
            }
            itr = m_pendingConnections.erase(itr);
        }
    }
}

void ServerEngine::onInteraction(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto& player = m_world.findEntity(client.getPlayerId());
    auto& position = player.position;
    auto& rotation = player.rotation;

    // See if a block was clicked
    auto result = m_world.tryInteract(InteractionKind::PlaceBlock, position, rotation);
    if (result) {
        VoxelUpdate update;
        update.voxelPosition = *result;
        update.voxel = m_world.getVoxelData().getVoxelId(CommonVoxel::Stone);

        // Send to clients that own this chunk
        for (auto& session : m_clients) {
            if (session.isActive()) {
                session.sendVoxelUpdate(update);
            }
        }
    }
}

void ServerEngine::onMouseState(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto& player = m_world.findEntity(client.getPlayerId());
    bool click = packet.read<u8>();
    if (click) {
        auto& position = player.position;
        auto& rotation = player.rotation;

        // See if a block was clicked
        auto result = m_world.tryInteract(InteractionKind::DigBlock, position, rotation);
        if (result) {
            VoxelUpdate update;
            update.voxelPosition = *result;
            update.voxel = m_world.getVoxelData().getVoxelId(CommonVoxel::Air);

            // Send to clients that own this chunk
            for (auto& session : m_clients) {
                if (session.isActive()) {
                    session.sendVoxelUpdate(update);
                }
            }
        }
    }
}

void ServerEngine::onPlayerState(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto position = packet.read<glm::vec3>();
    auto rotation = packet.read<glm::vec3>();

    auto& player = m_world.findEntity(client.getPlayerId());
    player.lastPosition = player.position;
    player.lastRotation = player.rotation;

    player.position = position;
    player.rotation = rotation;
}

void ServerEngine::onSpawnRequest(ServerPacket& packet, ENetPeer* peer)
{
    AUTHENTICATE_PACKET
    auto id = client.getPlayerId();
    client.sendPlayerSpawnPoint(m_world.getPlayerSpawnPosition(id));
}

void ServerEngine::handleDisconnection(ENetPeer* peer)
{
    auto itr = m_clientsMap.find(peer->incomingPeerID);
    if (itr != m_clientsMap.end()) {
        auto index = itr->second;
        m_world.removeEntity(m_clients[index].getPlayerId());
        m_clients[index].disconnect();
        broadcastPlayerLeave(m_clients[index].getPlayerId());
        m_clientsMap.erase(itr);
    }
    else {
        auto pending = findPendingSession(peer->incomingPeerID);
        if (pending != m_pendingConnections.end()) {
            m_pendingConnections.erase(pending);
        }
    }
}
