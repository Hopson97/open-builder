#include "client.h"

#include "../game/client_world.h"


void Client::onHandshakeChallenge(ClientPacket& packet)
{
    u32 salt = packet.read<u32>();
    u32 newSalt = m_salt ^ salt;
    m_salt = newSalt;
    ClientPacket response(ServerCommand::HandshakeResponse, m_salt);
    m_serverConnection.send(response.get());
}

void Client::onConnectionAcceptance(ClientPacket& packet)
{
    u8 isAccepted = packet.read<u8>();
    if (isAccepted) {
        std::cout << "Connected!\n";
        m_connectionState = ConnectionState::Connected;

        u32 playerId = packet.read<u32>();
        mp_world->setPlayerId(playerId);

        u32 count = packet.read<u32>();
        for (u32 i = 0; i < count; i++) {
            u32 id = packet.read<u32>();
            auto position = packet.read<glm::vec3>();
            auto rotation = packet.read<glm::vec3>();
            mp_world->addEntity(id, position, rotation);
        }
    }
    else {
        std::string reason = packet.read<std::string>();
        std::cout << "Rejected!\n" << reason << std::endl;
        m_connectionState = ConnectionState::Disconnected;
    }
}

void Client::onAddEntity(ClientPacket& packet)
{
    std::cout << "Player joined!\n";
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        u32 entityId = packet.read<u32>();
        glm::vec3 position = packet.read<glm::vec3>();
        glm::vec3 rotation = packet.read<glm::vec3>();
        mp_world->addEntity(entityId, position, rotation);
    }
}

void Client::onRemoveEntity(ClientPacket& packet)
{
    u32 entityId = packet.read<u32>();
    mp_world->removeEntity(entityId);
    std::cout << "Player left!\n";
}

void Client::onForceExit(ClientPacket& packet)
{
    m_connectionState = ConnectionState::Disconnected;
    auto reason = packet.read<std::string>();
    std::cout << "Forced to leave game: " << reason << "\n";
}

void Client::onUpdateEntityStates(ClientPacket& packet)
{
    u32 count = packet.read<u32>();
    for (u32 i = 0; i < count; i++) {
        auto entId = packet.read<u32>();
        auto entPosition = packet.read<glm::vec3>();
        auto entRotation = packet.read<glm::vec3>();
        if (entId == mp_world->getPlayerId()) {
            continue;
        }
        mp_world->updateEntity(entId, entPosition, entRotation);
    }
}